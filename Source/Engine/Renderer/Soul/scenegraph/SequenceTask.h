/*
 * Soul: a small object-oriented OpenGL Rendering Kernel.
 * Website : http://Soul.gforge.inria.fr/
 * Copyright (c) 2008-2015 INRIA - LJK (CNRS - Grenoble University)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors 
 * may be used to endorse or promote products derived from this software without 
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 * Soul is distributed under the BSD3 Licence. 
 * For any assistance, feedback and remarks, you can check out the 
 * mailing list on the project page : 
 * http://Soul.gforge.inria.fr/
 */
/*
 * Main authors: Eric Bruneton, Antoine Begault, Guillaume Piolat.
 */

#ifndef _Soul_SEQUENCE_TASK_H_
#define _Soul_SEQUENCE_TASK_H_

#include "Soul/scenegraph/AbstractTask.h"

namespace Soul
{

/**
 * An AbstractTask to compose a list of tasks in a sequence.
 * @ingroup scenegraph
 */
class Soul_API SequenceTask : public AbstractTask
{
public:
    /**
     * Creates a SequenceTask with the given list of tasks.
     *
     * @param subtasks the tasks that must be composed into a sequence.
     */
    SequenceTask(const std::vector< ptr<TaskFactory> > &subtasks);

    /**
     * Deletes this SequenceTask.
     */
    virtual ~SequenceTask();

    virtual ptr<Task> getTask(ptr<Object> context);

protected:
    /**
     * Creates an empty SequenceTask.
     */
    SequenceTask();

    /**
     * Initializes this SequenceTask with the given list of tasks.
     *
     * @param subtasks the tasks that must be composed into a sequence.
     */
    void init(const std::vector< ptr<TaskFactory> > &subtasks);

    /**
     * Swaps this SequenceTask with another one.
     *
     * @param t a SequenceTask.
     */
    void swap(ptr<SequenceTask> t);

private:
    /**
     * The tasks that are composed sequentially by this task.
     */
    std::vector< ptr<TaskFactory> > subtasks;
};

}

#endif