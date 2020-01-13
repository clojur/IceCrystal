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

#ifndef _LargescaleScene_DRAW_TWEAK_BAR_TASK_H_
#define _LargescaleScene_DRAW_TWEAK_BAR_TASK_H_
#include "LargescaleSceneMacro.h"
#include "Soul/scenegraph/AbstractTask.h"

using namespace Soul;

namespace LargescaleScene
{

/**
 * @defgroup twbar twbar
 * Provides classes to build a user interface using the AntTweakBar library.
 * @ingroup LargescaleScene_ui
 */

/**
 * An Soul::AbstractTask to display tweak bars.
 * @ingroup twbar
 * @authors Eric Bruneton, Antoine Begault
 */
class LargescaleScene_API  DrawTweakBarTask : public AbstractTask
{
public:
    /**
     * Creates a new DrawTweakBarTask.
     */
    DrawTweakBarTask();

    /**
     * Deletes this DrawTweakBarTask.
     */
    virtual ~DrawTweakBarTask();

    virtual ptr<Task> getTask(ptr<Object> context);

protected:
    void swap(ptr<DrawTweakBarTask> a);

private:
    /**
     * A Task to display the tweak bar.
     */
    class Impl : public Task
    {
    public:
        /**
         * Creates a new DrawTweakBarTask::Impl task.
         */
        Impl();

        /**
         * Deletes this DrawTweakBarTask::Impl.
         */
        virtual ~Impl();

        virtual bool run();
    };
};

}

#endif
