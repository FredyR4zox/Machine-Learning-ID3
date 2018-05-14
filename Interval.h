#ifndef INTERVAL_H
#define INTERVAL_H

class Interval {
private:
    double min;
    double max;
    std::string str;

public:
    Interval(double min, double max);

    bool isInside(double value);
    std::string getString();
};

Interval::Interval(double min, double max){
    this->min = min;
    this->max = max;
    
    std::stringstream stream, stream2;
    stream << std::fixed << std::setprecision(2) << min;
    stream2 << std::fixed << std::setprecision(2) << max;
    std::string firstInterval = stream.str();
    std::string secondInterval = stream2.str();

    if(min == std::numeric_limits<double>::min())
        firstInterval = "-inf";
    if(max == std::numeric_limits<double>::max())
        secondInterval = "+inf";

    if(min == std::numeric_limits<double>::min())
        this->str = firstInterval + " < x < " + secondInterval;
    else
        this->str = firstInterval + " <= x < " + secondInterval;
}

bool Interval::isInside(double value){
    if(value < this->min || value > this->max)
        return false;

    return true;
}

std::string Interval::getString(){
    return this->str;
}



#endif