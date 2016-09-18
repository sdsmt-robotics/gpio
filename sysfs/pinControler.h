#include <fstream>

using namespace std;

class PinCont
{
  
  public:
    PinCont();
    ~PinCont();
    void pinState(int num, bool state);
    int getValue(int num);
    void pinOn(int num);
    void pinOff(int num);
  
  
  private:
  
    std::ofstream importFiles;
    std::ofstream exportFiles;                           

    const  int filesSize = 42;
    
    const int pinNumber[42] = {-1, -1, -1, 173, 174, 171, 192, 172, 191, 189, 190, -1, 21, 210, 18, 209, 22, 19, 30, 28, 29, 31, -1, 25, 23, 24, 33, -1, -1, -1, -1, -1, -1, 187, 34, 188, 225, -1, 226, 229, 227, 228};

    bool pin[42];
    
    std::ofstream directionFiles[42];

    std::ifstream valueFilesIn[42];

    std::ofstream valueFilesOut[42];
};




int PinCont::getValue(int num)
{

  valueFilesIn[num - 1].seekg(0, std::ios::beg);
  int temp;
  valueFilesIn[num - 1] >> temp;
  return temp;
}

PinCont::PinCont()
{
  importFiles.open("/sys/class/gpio/export");
  exportFiles.open("/sys/class/gpio/unexport");
}

PinCont::~PinCont()
{
  

  for(int i = 0; i < filesSize; i++)
  {
    valueFilesOut[i].close();
  }
  
  for(int i = 0; i < filesSize; i++)
  {
    valueFilesIn[i].close();
  }
  
  for(int i = 0; i < filesSize; i++)
  {
    directionFiles[i].close();
    pin[i] = false;
  }
  
  exportFiles.close();
  importFiles.close();
}   

void PinCont::pinOn(int num)
{

  valueFilesOut[num - 1] << "1" << std::endl;

}

void PinCont::pinOff(int num)
{

  valueFilesOut[num - 1] << "0" << std::endl;

}

//Num is the pin number
//Create, open and sets the pen state
//State is if it is a input or output pin
//  true  = input pin
//  false = output pin

void PinCont::pinState(int num, bool state)
{

  
  bool open = false;
  if(directionFiles[num - 1].is_open())
  {
    open = true;
  }

  if(open)
  {
    if(pin[num - 1])
    {
      if(state)
        return;
      pin[num - 1] = state;
      valueFilesIn[num - 1].close();
      valueFilesOut[num - 1].open("/sys/class/gpio/gpio" + std::to_string(pinNumber[num - 1]) + "/value");
      directionFiles[num - 1] << "out" << std::endl;
    }else
    {
      if(!state)
        return;
      pin[num - 1] = state;
      valueFilesOut[num - 1].close();
      valueFilesIn[num - 1].open("/sys/class/gpio/gpio" + std::to_string(pinNumber[num - 1]) + "/value");
      directionFiles[num - 1] << "in" << std::endl;
    }
    
  //create the pin
  }else
  {
    
    importFiles << pinNumber[num - 1] << std::endl;
    std::string fileLocation = "/sys/class/gpio/gpio" + std::to_string(pinNumber[num - 1]) + "/direction";
    directionFiles[num - 1].open(fileLocation.c_str());
    pin[num - 1] = state;
    if(state)
    {
      directionFiles[num - 1] << "in" << std::endl;
      std::string fileLocation = "/sys/class/gpio/gpio" + std::to_string(pinNumber[num - 1]) + "/value";
      valueFilesIn[num - 1].open(fileLocation);
    }else
    {
      directionFiles[num - 1] << "out" << std::endl;
      std::string fileLocation = "/sys/class/gpio/gpio" + std::to_string(pinNumber[num - 1]) + "/value";
      valueFilesOut[num - 1].open(fileLocation);
    }
  }
}
