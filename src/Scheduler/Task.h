//
// Created by moritz on 17.06.19.
//

#ifndef EMBEDDEDSYSTEMSPROJECT_TASK_H
#define EMBEDDEDSYSTEMSPROJECT_TASK_H

#include <string>

namespace SCHEDULER {
    class Task {
    public:
        Task();
        int getId();
        void* getData();
        void addData(void* value);
        void setFile(std::string fileName);
        void setKernel(std::string kernelName);
    private:

    };
}

#endif //EMBEDDEDSYSTEMSPROJECT_TASK_H
