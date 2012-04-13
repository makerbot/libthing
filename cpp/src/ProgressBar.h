
#ifndef _PROGRESS_BAR_H_
#define _PROGRESS_BAR_H_ (1)


//
// ASCII art
//
#include "PlatformTools.h"

namespace libthing {

class ProgressBar
{

    unsigned int count;
    unsigned int ticks;

protected:
    std::string task;
 public:
    ProgressBar(unsigned int count=0, const char* taskName="")
    {
        reset(count, taskName);
    }

    virtual ~ProgressBar(){};

    void reset(unsigned int count, const char* taskName = "")
    {
        ticks = 0;
        this->count = count;
        task = taskName;
    }

    void tick()
    {
        onTick(task.c_str(), count, ticks);
        ticks++;
    }

    virtual void onTick(const char* taskName, unsigned int size, unsigned int it)=0;

};


class ProgressLog : public ProgressBar
{
public:
	OperatingSystem myPc;
    unsigned int delta;
    unsigned int deltaTicks;
    unsigned int deltaProgress;

        ProgressLog(unsigned int count=0);
        void onTick(const char* taskName, unsigned int count, unsigned int tick);
};

}
#endif /*_PROGRESS_BAR_H_*/
