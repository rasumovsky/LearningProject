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

#include "Axon.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

class Axon;

class Neuron {
  
 public:
  
  Neuron(int layerIndex, std::string function);
  ~Neuron();
  
  // Public Accessors:
  double getDelta();
  int getLayerIndex();
  double getResponse();
  double getResponseDerivative();
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
  void backPropagation();
  void clearDelta();
  void clearResponse();
  //void clearResponseSum();
  void setBiasNode(bool biasNode);
  void setLayerIndex(int layerIndex);
  void setResponse(double response);
  void setResponseWithSum(double sum);
  void setTarget(double target);
  
 private:
  
  // Private functions:
  double thresholdDerivative(double sum);
  double thresholdFunction(double sum);
  
  // Member objects:
  std::vector<Axon*> m_downstreamConnections;
  std::vector<Axon*> m_upstreamConnections;
  std::string m_function;
  
  int m_layerIndex;
  bool m_biasNode;
  bool m_hasResponse;
  double m_response;
  double m_responseDerivative;
  //double m_sumOfResponses;
  bool m_hasDelta;
  double m_delta;
  double m_target;
};

#endif
