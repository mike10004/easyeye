/*
 * portability.h
 */

#ifndef PORTABILITY_H_
#define PORTABILITY_H_

namespace portability
{

class Math
{
public:
    static bool IsNaN(const double x);
    static double GetNaN();
private:
    Math();
};
    
}

#endif /* PORTABILITY_H_ */
