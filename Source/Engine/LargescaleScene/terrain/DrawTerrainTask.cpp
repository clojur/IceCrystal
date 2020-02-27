/*
 * LargescaleScene: a procedural landscape rendering library.
 * Copyright (c) 2008-2011 INRIA
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * LargescaleScene is distributed under a dual-license scheme.
 * You can obtain a specific license from Inria: LargescaleScene-licensing@inria.fr.
 */

/*
 * Authors:jiangsi.
 */

#include "LargescaleScene/terrain/DrawTerrainTask.h"

#include "Soul/resource/ResourceTemplate.h"
#include "Soul/render/FrameBuffer.h"

using namespace std;
using namespace Soul;

namespace LargescaleScene
{

DrawTerrainTask::DrawTerrainTask() : AbstractTask("DrawTerrainTask")
{
}

DrawTerrainTask::DrawTerrainTask(const QualifiedName &terrain, const QualifiedName &mesh, bool culling) :
    AbstractTask("DrawTerrainTask")
{
    init(terrain, mesh, culling);
}

void DrawTerrainTask::init(const QualifiedName &terrain, const QualifiedName &mesh, bool culling)
{
    this->terrain = terrain;
    this->mesh = mesh;
    this->culling = culling;
}

DrawTerrainTask::~DrawTerrainTask()
{
}

ptr<Task> DrawTerrainTask::getTask(ptr<Object> context)
{
    ptr<SceneNode> n = context.cast<Method>()->getOwner();

    ptr<TerrainNode> t = NULL;
    ptr<SceneNode> target = terrain.getTarget(n);
    if (target == NULL) {
        t = n->getOwner()->getResourceManager()->loadResource(terrain.name).cast<TerrainNode>();
    } else {
        t = target->getField(terrain.name).cast<TerrainNode>();
    }
    if (t == NULL) {
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("TERRAIN", "DrawTerrain : cannot find terrain '" + terrain.target + "." + terrain.name + "'");
        }
        throw exception();
    }

    ptr<MeshBuffers> m = NULL;
    target = mesh.getTarget(n);
    if (target == NULL) {
        m = n->getOwner()->getResourceManager()->loadResource(mesh.name + ".mesh").cast<MeshBuffers>();
    } else {
        m = target->getMesh(mesh.name);
    }
    if (m == NULL) {
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("SCENEGRAPH", "DrawMesh : cannot find mesh '" + mesh.target + "." + mesh.name + "'");
        }
        throw exception();
    }
    return new Impl(n, t, m, culling);
}

void DrawTerrainTask::swap(ptr<DrawTerrainTask> t)
{
    std::swap(*this, *t);
}

DrawTerrainTask::Impl::Impl(ptr<SceneNode> pSceneNode, ptr<TerrainNode> pTerrainNode, ptr<MeshBuffers> pMeshBuffers, bool bCulling) :
    Task("DrawTerrain", true, 0), 
	_pSceneNode(pSceneNode),
	_pTerrainNode(pTerrainNode),
	_pMeshBuffers(pMeshBuffers),
	_bCulling(bCulling)
{
}

DrawTerrainTask::Impl::~Impl()
{
}

bool DrawTerrainTask::Impl::run()
{
    if (_pTerrainNode != NULL) {
        if (Logger::DEBUG_LOGGER != NULL) {
            Logger::DEBUG_LOGGER->log("TERRAIN", "DrawTerrain");
        }
        ptr<FrameBuffer> fb = SceneManager::getCurrentFrameBuffer();
        _bAsync = false;
        vector< ptr<TileSampler> > uniforms;
        SceneNode::FieldIterator i = _pSceneNode->getFields();
        while (i.hasNext()) {
            ptr<TileSampler> u = i.next().cast<TileSampler>();
            if (u != NULL) {
                if (u->getTerrain(0) != NULL) {
                    u->setTileMap();
                }
                if (u->getStoreLeaf() && u->getTerrain(0) == NULL) {
                    uniforms.push_back(u);
                    if (u->getAsync() && !u->getMipMap()) {
                        _bAsync = true;
                    }
                }
            }
        }

        ptr<Program> pShaderProgram = SceneManager::getCurrentProgram();
        _pTerrainNode->deform->setUniforms(_pSceneNode, _pTerrainNode, pShaderProgram);
        if (_bAsync) {
            int k = 0;
            switch (_pMeshBuffers->mode) {
                case TRIANGLES:
                    k = 6;
                    break;
                case TRIANGLES_ADJACENCY:
                    k = 12;
                    break;
                case LINES_ADJACENCY:
                case PATCHES:
                    k = 4;
                    break;
                default:
                    // unsupported formats
                    assert(false);
            }
            int n = int(sqrt((double)_pMeshBuffers->nvertices)) - 1;
            _iGridSize = (n / 2) * (n / 2) * k;
            assert(_pMeshBuffers->nindices >= _iGridSize * 32);

            findDrawableQuads(_pTerrainNode->root, uniforms);
        }
        drawQuad(_pTerrainNode->root, uniforms);
    }
    return true;
}

void DrawTerrainTask::Impl::findDrawableQuads(ptr<TerrainQuad> pTerrainQuad, const std::vector< ptr<TileSampler> > &uniforms)
{
	pTerrainQuad->drawable = false;

    if (_bCulling && pTerrainQuad->visible == SceneManager::INVISIBLE) {
		pTerrainQuad->drawable = true;
        return;
    }

    if (pTerrainQuad->isLeaf()) {
        for (unsigned int i = 0; i < uniforms.size(); ++i) {
            if (!uniforms[i]->getAsync() || uniforms[i]->getMipMap()) {
                continue;
            }
            ptr<TileProducer> p = uniforms[i]->get();
            if (p->hasTile(pTerrainQuad->level, pTerrainQuad->tx, pTerrainQuad->ty) && p->findTile(pTerrainQuad->level, pTerrainQuad->tx, pTerrainQuad->ty) == NULL) {
                return;
            }
        }
    } else {
        int nDrawable = 0;
        for (int i = 0; i < 4; ++i) {
            findDrawableQuads(pTerrainQuad->children[i], uniforms);
            if (pTerrainQuad->children[i]->drawable) {
                ++nDrawable;
            }
        }
        if (nDrawable < 4) {
            for (unsigned int i = 0; i < uniforms.size(); ++i) {
                if (!uniforms[i]->getAsync() || uniforms[i]->getMipMap()) {
                    continue;
                }
                ptr<TileProducer> p = uniforms[i]->get();
                if (p->hasTile(pTerrainQuad->level, pTerrainQuad->tx, pTerrainQuad->ty) && p->findTile(pTerrainQuad->level, pTerrainQuad->tx, pTerrainQuad->ty) == NULL) {
                    return;
                }
            }
        }
    }

	pTerrainQuad->drawable = true;
}

void DrawTerrainTask::Impl::drawQuad(ptr<TerrainQuad> pTerrainQuad, const vector< ptr<TileSampler> > &uniforms)
{
    if (_bCulling && pTerrainQuad->visible == SceneManager::INVISIBLE) {
        return;
    }
    if (_bAsync && !pTerrainQuad->drawable) {
        return;
    }

    ptr<Program> p = SceneManager::getCurrentProgram();
    if (pTerrainQuad->isLeaf()) {
        for (unsigned int i = 0; i < uniforms.size(); ++i) {
            uniforms[i]->setTile(pTerrainQuad->level, pTerrainQuad->tx, pTerrainQuad->ty);
        }
        _pTerrainNode->deform->setUniforms(_pSceneNode, pTerrainQuad, p);
        if (_bAsync) {
            SceneManager::getCurrentFrameBuffer()->draw(p, *_pMeshBuffers, _pMeshBuffers->mode, 0, _iGridSize * 4);
        } else {
            if (_pMeshBuffers->nindices == 0) {
                SceneManager::getCurrentFrameBuffer()->draw(p, *_pMeshBuffers, _pMeshBuffers->mode, 0, _pMeshBuffers->nvertices);
            } else {
                SceneManager::getCurrentFrameBuffer()->draw(p, *_pMeshBuffers, _pMeshBuffers->mode, 0, _pMeshBuffers->nindices);
            }
        }
    } else {
        int order[4];
        double ox = _pTerrainNode->getLocalCamera().x;
        double oy = _pTerrainNode->getLocalCamera().y;

        double cx = pTerrainQuad->ox + pTerrainQuad->l / 2.0;
        double cy = pTerrainQuad->oy + pTerrainQuad->l / 2.0;
        if (oy < cy) {
            if (ox < cx) {
                order[0] = 0;
                order[1] = 1;
                order[2] = 2;
                order[3] = 3;
            } else {
                order[0] = 1;
                order[1] = 0;
                order[2] = 3;
                order[3] = 2;
            }
        } else {
            if (ox < cx) {
                order[0] = 2;
                order[1] = 0;
                order[2] = 3;
                order[3] = 1;
            } else {
                order[0] = 3;
                order[1] = 1;
                order[2] = 2;
                order[3] = 0;
            }
        }

        int done = 0;
        for (int i = 0; i < 4; ++i) {
            if (_bCulling && pTerrainQuad->children[order[i]]->visible == SceneManager::INVISIBLE) {
                done |= (1 << order[i]);
            } else if (!_bCulling || pTerrainQuad->children[order[i]]->drawable) {
                drawQuad(pTerrainQuad->children[order[i]], uniforms);
                done |= (1 << order[i]);
            }
        }
        if (done < 15) {
            int sizes[16] = { 0, 4, 7, 10, 12, 15, 17, 19, 20, 23, 25, 27, 28, 30, 31, 32 };
            for (unsigned int i = 0; i < uniforms.size(); ++i) {
                uniforms[i]->setTile(pTerrainQuad->level, pTerrainQuad->tx, pTerrainQuad->ty);
            }
            _pTerrainNode->deform->setUniforms(_pSceneNode,pTerrainQuad, p);
            SceneManager::getCurrentFrameBuffer()->draw(p, *_pMeshBuffers, _pMeshBuffers->mode, _iGridSize * sizes[done], _iGridSize * (sizes[done+1] - sizes[done]));
        }
    }
}

class DrawTerrainTaskResource : public ResourceTemplate<40, DrawTerrainTask>
{
public:
    DrawTerrainTaskResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<40, DrawTerrainTask>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        checkParameters(desc, e, "name,mesh,culling,");
        string n = getParameter(desc, e, "name");
        string m = getParameter(desc, e, "mesh");
        bool culling = false;
        if (e->Attribute("culling") != NULL && strcmp(e->Attribute("culling"), "true") == 0) {
            culling = true;
        }
        init(QualifiedName(n), QualifiedName(m), culling);
    }
};

extern const char drawTerrain[] = "drawTerrain";

static ResourceFactory::Type<drawTerrain, DrawTerrainTaskResource> DrawTerrainTaskType;

}
