////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Name: Neuron.h                                                            //
//  Class: Neuron.cxx                                                         //
//                                                                            //
//  Author: Andrew Hard                                                       //
//  Email: ahard@cern.ch                                                      //
//  Date: 24/02/2015                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef Neuron_h
#define Neuron_h

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include "Axon.h"

class Neuron 
{

 public:
  
  Neuron(std::vector<*Axon> *newConnections, std::string newFunction);
  Neuron(std::vector<*Axon> *newConnections);
  Neuron();
  ~Neuron();
  
  // Mutators:
  void addConnection(Axon *newAxon);
  void clearBits();
  void randomizeWeights();
  void removeAllConnections();
  Axon* removeConnection(Axon *oldAxon);
  void setResponse(double newResponse);
  
  // Accessors:
  double evaluateResponse();
  int getNConnections();
  double getStaticResponse();
  bool hasResponse();
  bool isRandom();
  
 private:
  
  // Member objects:
  bool randomBit;
  bool responseBit;
  
  double response;
  std::vector<*Axon> *connections;
  std::string function;
  
  // private functions:
  double thresholdFunction(double sum);
  
};

#endif
