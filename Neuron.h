////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Name: Neuron.h                                                            //
//  Class: Neuron.cxx                                                         //
//                                                                            //
//  Author: Andrew Hard                                                       //
//  Email: ahard@cern.ch                                                      //
//  Date: 18/09/2015                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef Neuron_h
#define Neuron_h

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include "Axon.h"

class Neuron {

 public:
  
  Neuron(int layerIndex, std::string function);
  ~Neuron();
  
  // Public Accessors:
  void clearResponse();
  int getLayerIndex();
  double getResponse();
  int getNDownstreamConnections();
  int getNUpstreamConnections();
  std::vector<Axon*> getDownstreamConnections();
  std::vector<Axon*> getUpstreamConnections();
  bool isBiasNode();
  bool isInputNode();
  bool isOutputNode();
  
  // Public Mutators:
  void addDownstreamConnection(Axon *axon);
  void addDownstreamConnection(std::vector<Axon*> axons);
  void addUpstreamConnection(Axon *axon);
  void addUpstreamConnection(std::vector<Axon*> axons);
  void setLayerIndex(int layerIndex);
  
 private:
  
  // Private functions:
  double thresholdFunction(double sum);
  
  // Member objects:
  std::vector<*Axon> m_downstreamConnections;
  std::vector<*Axon> m_upstreamConnections;
  std::string m_function;
  
  bool m_biasNode;
  bool m_hasResponse;
  bool m_response;
  
};

#endif
