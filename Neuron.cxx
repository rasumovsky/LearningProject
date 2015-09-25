////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Name: Neuron.cxx                                                          //
//                                                                            //
//  Created: Andrew Hard                                                      //
//  Email: ahard@cern.ch                                                      //
//  Date: 18/09/2015                                                          //
//                                                                            //
//  This class represents a node in a neural network. The connections are to  //
//  upstream and downstream nodes. Activation functions are included, as well //
//  as the derivatives, response, and back-propagation utilities.             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "Neuron.h"

/**
   -----------------------------------------------------------------------------
   Neuron constructor, with option to set the threshold function:
   @param layerIndex - The index of the layer in which this Neuron resides.
   @param function - The activation function for the neuron: "sigmoid", "tanh",
   "linear", all defined for x:[-1,+1], y:[-1,+1].
*/
Neuron::Neuron(int layerIndex, std::string function) {
  setBiasNode(false);
  m_hasResponse = false;
  m_response = 0.0;
  m_responseDerivative = 0.0;
  //m_sumOfResponses = 0.0;
  m_hasDelta = false;
  m_delta = 0.0;
  m_downstreamConnections.clear();
  m_upstreamConnections.clear();
  m_function = function;
  m_target = 0.0;
  return;
}

/**
   -----------------------------------------------------------------------------
   Add a single downstream connection.
   @param axon - A single downstream connection.
*/
void Neuron::addDownstreamConnection(Axon *axon) {
  if (!axon) {
    std::cout << "Neuron: ERROR! axon is null" << std::endl;
    exit(0);
  }
  m_downstreamConnections->push_back(axon);
}

/**
   -----------------------------------------------------------------------------
   Add multiple downstream connections.
   @param axons - A vector of downstream connections.
*/
void Neuron::addDownstreamConnection(std::vector<Axon*> axons) {
  std::vector<Axon*>::iterator axonIter;
  for (axonIter = axons.begin(); axonIter != axons.end(); axonIter++) {
    addDownstreamConnection(*axonIter);
  }
}

/**
   -----------------------------------------------------------------------------
   Add a single upstream connection.
   @param axon - A single upstream connection.
*/
void Neuron::addUpstreamConnection(Axon *axon) {
  if (!axon) {
    std::cout << "Neuron: ERROR! axon is null" << std::endl;
    exit(0);
  }
  else if (isBiasNode()) {
    std::cout << "Error: cannot add upstream connection to bias Node." 
	      << std::endl;
    exit(0);
  }
  else {
    m_upstreamConnections->push_back(axon);
  }
}

/**
   -----------------------------------------------------------------------------
   Add multiple upstream connections.
   @param axons - A vector of upstream connections.
*/
void Neuron::addUpstreamConnection(std::vector<Axon*> axons) {
  for (std::vector<Axon*>::iterator axonIter = axons.begin();
       axonIter != axons.end(); axonIter++) {
    addUpstreamConnection(*axonIter);
  }
}

/**
   -----------------------------------------------------------------------------
   This algorithm recursively calls itself on ancestor (upstream) Neurons. Once
   it reaches a bias node or input node with no prior weights or nodes, it 
   calls the getDelta() function, which recursively updates the weights and 
   deltas for the following graph. Flags prevent duplication of calculation and
   also conflicts arising from weight reassignment. 
*/
void backPropagation() {
  if (isBiasNode() || isInputNode()) {
    getDelta();
  }
  // Call back-propagation on previous neuron layer.
  else {
    // Then update the weights for the following connections.
    for (std::vector<Axon*>::iterator axonIter = m_upstreamConnections.begin(); 
	 axonIter != m_upstreamConnections.end(); axonIter++) {
      axonIter->getOriginNeuron()->backPropagation();
    }
  }
}

/**
   -----------------------------------------------------------------------------
   Clears the delta value information, which is stored for speed. Note: this 
   algorithm is recursive. It will clear the delta values for all downstream 
   (predecessor) Neurons.
*/
void Neruon::clearDelta() {
  if (m_hasDelta) {
    m_hasDelta = false;
    m_delta = 0.0;
    // Then recursively clear predecesor Neurons' deltas:
    std::vector<Axon*> predecessorConnections = getDownstreamConnections();
    for (std::vector<Axon*>::iterator axonIter = predecessorConnections.begin();
	 axonIter != predecessorConnections.end(); axonIter++) {
      axonIter->getTerminalNeuron()->clearDelta();
    }
  }
}

/**
   -----------------------------------------------------------------------------
   Clears the response information, which is stored for speed.
*/
void Neuron::clearResponse() {
  m_hasResponse = false;
  m_response = 0.0;
  m_responseDerivative = 0.0;
}

/**
   -----------------------------------------------------------------------------
   Clears the response sum information, which is stored for speed.
*/
//void Neuron::clearResponseSum() {
//  m_sumOfResponses = 0.0;
//}

/**
   -----------------------------------------------------------------------------
   Get the delta value associated with this Neuron for back-propagation. Relies
   on the downstream connection weights and downstream connection deltas.
   
   MUST BE MODIFIED TO USE SUM! AND SUM FOR DERIVATIVE!
*/
double Neuron::getDelta() {
  m_delta = 0.0;
  double derivative = getResponseDerivative();
  if (m_hasDelta) {
    return m_delta;
  }
  else if (isOutputNode()) {
    m_delta = ((m_response - m_target) * derivative);
  }
  else {
    // first sum up:
    double currSum = 0.0;
    for (int i_c = 0; i_c < (int)m_downstreamConnections.size(); i_c++) {
      double currWeight = m_downstreamConnections[i_c]->getWeight();
      double currDelta
	= (m_downstreamConnections[i_c])->getTerminalNeuron()->getDelta();
      currSum += (currWeight * currDelta);
    }
    // Then multiply sum by derivative:
    m_delta = (currSum * derivative);
  }
  m_hasDelta = true;
  // Then update the downstream weights:
  for (std::vector<Axon*>::iterator axonIter = m_downstreamConnections.begin(); 
       axonIter != m_downstreamConnections.end(); axonIter++) {
    axonIter->trainWeight();
  }
  return m_delta;
}

/**
   -----------------------------------------------------------------------------
   @returns - A vector of downstream connections for the Neuron.
*/
std::vector<Axon*> Neuron::getDownstreamConnections() {
  return m_downstreamConnections;
}

/**
   -----------------------------------------------------------------------------
   @returns - The number of downstream connections for the Neuron.
*/
int Neuron::getNDownstreamConnections() {
  return (int)m_downstreamConnections.size();
}

/**
   -----------------------------------------------------------------------------
   @returns - The number of upstream connections for the Neuron.
*/
int Neuron::getNUpstreamConnections() {
  return (int)m_upstreamConnections.size();
}

/**
   -----------------------------------------------------------------------------
   Evaluate the neuron response based on the current input connection weights.
   @returns - The neuron response, which is 1 for bias nodes, equal to the input
   for input nodes, and is variable for all other nodes. 
*/
double Neuron::getResponse() {
  if (!m_hasResponse) {
    // Set the response to 1 for bias nodes:
    if (isBiasNode()) {
      m_response = 1.0;
      m_responseDerivative = 0.0;
      //m_sumOfResponses += 1.0;
    }
    // If the input node doesn't have a response, exit.
    else if (isInputNode()) {
      std::cout << "Neuron: Error! Input node response undefined!" << std::endl;
      exit(0);
    }  
    // Calculate new response:
    else {
      double currSum = 0.0;
      // Loop over input connections
      for (std::vector<Axon*>::iterator axonIter =m_upstreamConnections.begin();
	   axonIter != m_upstreamConnections.end(); axonIter++) {
	// sum the input weights:
	double inputWeight = axonIter->getWeight();
	double inputResponse = axonIter->getOriginNeuron()->getResponse();
	currSum += (inputWeight * inputResponse);
      }
      // Then get the result of the threshold function:
      m_response = thresholdFunction(currSum);
      m_responseDerivative = thresholdDerivative(currSum);
      //m_sumOfResponses += currSum;
    }
    m_hasResponse = true;
  }
  return m_response;
}

/**
   -----------------------------------------------------------------------------
   Check to see that a response (and its derivative) have been calculated. Then
   return the derivative. Note that the getResponse() function that is called is
   recursive. 
*/
double Neuron::getResponseDerivative() {
  if (!m_hasResponse()) {
    getResponse();
  }
  return m_responseDerivative;
}

/**
   -----------------------------------------------------------------------------
   @returns - A vector of upstream connections for the Neuron.
*/
std::vector<Axon*> Neuron::getUpstreamConnections() {
  return m_upstreamConnections;
}

/**
   -----------------------------------------------------------------------------
   Check whether this node is a (constant) bias node.
   @returns - True iff neuron is a bias node.
*/
bool Neuron::isBiasNode() {
  return m_biasNode();
}

/**
   -----------------------------------------------------------------------------
   Check whether this node is an input node (without predecessors and not bias 
   node).
   @returns - True iff neuron is an input node. 
*/
bool Neuron::isInputNode() {
  if (isBiasNode()) return false;
  else return (getNUpstreamConnections() == 0);
}

/**
   -----------------------------------------------------------------------------
   Check whether this node is an output node (without ancestors).
   @returns - True iff neuron is an output node.
*/
bool Neuron::isOutputNode() {
  return (getNDownstreamConnections() == 0);
}

/**
   -----------------------------------------------------------------------------
   Set this node to be a bias node or not.
   @param biasNode - True iff. this node should be a bias node.
*/
void Neuron::setBiasNode(bool biasNode) {
  m_biasNode = biasNode;
}

/**
   -----------------------------------------------------------------------------
   Set the response of this node. Mostly necessary for input nodes.
   WARNING: Commented because it is not clear how to set the derivative here...
   @param response - The new response of the Neuron.
*/
//void Neuron::setResponse(double response) {
//  m_response = response;
//  m_hasResponse = true;
//}

/**
   -----------------------------------------------------------------------------
   Set the response of this node. Mostly necessary for input nodes.
   @param sum - The new sum of inputs to the Neuron.
*/
void Neuron::setResponseWithSum(double sum) {
  m_response = thresholdFunction(sum);
  m_responseDerivative = thresholdDerivative(sum);
  m_hasResponse = true;
}

/**
   -----------------------------------------------------------------------------
   Set the target value. Note: This should only be done for output nodes.
   @param target - The new target for the output.
*/
void Neuron::setTarget(double target) {
  if (!isOutputNode()) {
    std::cout << "Neuron: Question! Why set target for non-output node?"
	      << std::endl;
    exit(0);
  }
  if (target < -1.0 || target > 1.0) {
    std::cout << "Neuron: ERROR! Target = " << target 
	      << " is out of [-1,+1] bounds!" << std::endl;
    exit(0);
  }
  m_target = target;
}

/**
   -----------------------------------------------------------------------------
   Evaluate the derivative of the specified threshold function for a given sum.
   @param sum - The sum of weights of the input connections.
   @returns - The derivative of the response for a given sum.
*/
double Neuron::thresholdDerivative(double sum) {
  if (m_function == "sigmoid") {
    return (thresholdFunction(sum) * (1.0 - thresholdFunction(sum)));
  }
  else if (m_function == "tanh") {
    return (-1.0 * thresholdFunction(sum) * thresholdFunction(sum));
  }
  else if (m_function == "linear") {
    return 1.0;
  }
  else if (m_function == "sine") {
    return ((3.141592653/2.0) * thresholdFunction(1.0 - sum));
  }
  else {
    std::cout << "Neuron: must define derivative for function " << m_function 
	      << std::endl;
    exit(0);
  }
}

/**
   -----------------------------------------------------------------------------
   Evaluate the threshold function to see whether the neuron should fire. The
   threshold functions are defined to be on an interval x:[-1,+1] with an output
   y:[-1,+1].
   @param sum - The sum of weights of the input connections.
   @returns - The response for a given sum.
*/
double Neuron::thresholdFunction(double sum) {
  if (m_function == "sigmoid") {
    return (1.0 / (1 + exp(-1.0*sum)));
  }
  else if (m_function == "tanh") {
    return ((exp(sum) - exp(-1.0*sum)) / (exp(sum) + exp(-1.0*sum)));
  }
  else if (m_function == "linear") {
    if (sum < -1.0) return -1.0;
    else if (sum > 1.0) return 1.0;
    else return sum;
  }
  else if (m_function == "sine") {
    if (sum < 1.0) return -1.0;
    else if (sum > 1.0) return 1.0;
    else return sin(3.141592653*sum/2.0);
  }
  else {
    std::cout << "Neuron: improperly assigned threshold function " << m_function
	      << std::endl;
    exit(0);
  }
}
