#ifndef SINK_H
#define SINK_H

enum class Sink {CONSOLE = 0, FILE, BOTH, EMPTY};

inline std::ostream& operator << (std::ostream& stm, Sink sink)
{
    switch(sink)
    {
        case Sink::CONSOLE : return stm << "CONSOLE";
        case Sink::FILE    : return stm << "FILE";
        case Sink::BOTH    : return stm << "BOTH";
        case Sink::EMPTY   : return stm << "EMPTY";
        default : return stm << "Sink{" << int(sink) << "}"; 
    }
}

inline std::string get_name(Sink sink) 
{
  switch (sink) 
  {
    case Sink::CONSOLE : return std::string("CONSOLE");
    case Sink::FILE    : return std::string("FILE");
    case Sink::BOTH    : return std::string("BOTH");
    case Sink::EMPTY   : return std::string("EMPTY");
    default : return std::string("");
  }
}

inline Sink operator++(Sink& x) 
{ 
    return x = (Sink)(((int)(x) + 1)); 
}

inline Sink operator*(Sink c) 
{
    return c;
}

inline Sink begin(Sink r) 
{
    return Sink::CONSOLE;
}

inline Sink end(Sink r)   
{
    return Sink(int(Sink::EMPTY) + 1);
}


#endif