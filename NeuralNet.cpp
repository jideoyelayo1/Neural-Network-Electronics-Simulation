
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;

//********** Training class ********** //
class TrainingData {
public:
    TrainingData(const string filename);
    bool isEof(void) { return _trainingDataFile.eof(); }
    void getTopology(vector<unsigned>& topology);
    unsigned getNextInputs(vector<double>& inputVals);
    unsigned getTargetOutputs(vector<double>& targetOutputsVals);

private:
    ifstream _trainingDataFile;
};

void TrainingData::getTopology(vector<unsigned>& topology) {
    string line;
    string label;

    if (!getline(_trainingDataFile, line)) {
        // Failed to read a line from the file. Handle the error.
        cerr << "Error reading from the file." << endl;
        abort();
    }

    stringstream ss(line);
    ss >> label;

    if (label != "topology:") {
        // Invalid format. Handle the error.
        cerr << "Invalid format. Expected 'topology:'." << endl;
        abort();
    }

    unsigned numLayers;
    while (ss >> numLayers) {
        topology.push_back(numLayers);
    }

    if (topology.empty()) {
        // No topology data found. Handle the error.
        cerr << "No topology data found." << endl;
        abort();
    }
}


TrainingData::TrainingData(const string filename) {
    _trainingDataFile.open(filename.c_str());
}

unsigned TrainingData::getNextInputs(vector<double>& inputVals) {
    inputVals.clear();
    string line;
    getline(_trainingDataFile, line);
    stringstream ss(line);
    string label;
    ss >> label;
    if (label.compare("in:") == 0) {
        double oneVal;
        while (ss >> oneVal) {
            inputVals.push_back(oneVal);
        }
    }
    return inputVals.size();
}

unsigned  TrainingData::getTargetOutputs(vector<double>& targetOutputsVals) {
    targetOutputsVals.clear();
    string line; 
    getline(_trainingDataFile, line);
    stringstream ss(line);
    string label;
    ss >> label;
    if (label.compare("out:") == 0) {
        double oneVal;
        while (ss >> oneVal) {
            targetOutputsVals.push_back(oneVal);
        }
    }
    return targetOutputsVals.size();
}

class Neuron;

typedef vector<Neuron> Layer;


//********** class Connection ********** //

struct Connection {
    double weight;
    double deltaWeight;
};

//********** class Neuron ********** //

class Neuron 
{
public:
    Neuron(unsigned numOutputs, unsigned myIdx);
    void setOutputVal(double val) { _outputVal = val; }
    //double getOutputVal(void) { return _outputVal; }
    double getOutputVal(void) const { return _outputVal; }
    void feedForward(const Layer &prevLayer);
    void calcOutputGradients(double targetVal);
    void calcHiddenGradients(const Layer &nextLayer);
    void updateInputWeights(Layer& prevLayer);
private:
    static double eta; // [0.0..1.0]
    static double alpha; // [0.0..1.0]
    static double randomWeight(void) { return rand() / double(RAND_MAX); }
    double _outputVal;
    vector<Connection> _outputWeights;
    unsigned _myIdx;
    static double transferFunction(double x);
    static double transferFunctionDerivative(double x);
    double _gradient;
    double sumDOW(const Layer& nextlayer) const;

};
double Neuron::eta = 0.15;
double Neuron::alpha = 0.5;

void Neuron::updateInputWeights(Layer& prevLayer) {
    // The weights to be updated

    for (unsigned n = 0; n < prevLayer.size(); ++n) {
        Neuron& neuron = prevLayer[n];
        double prevDeltaWeight = neuron._outputWeights[_myIdx].deltaWeight;

        double currDeltaWeight =
            eta // eta -> overall net learning rate: 0.0 slow learner < 0.2 medium learner < 1.0 reckless learner;
            * neuron.getOutputVal()
            * _gradient
            + alpha // momentum  0.0 -> no momentum : 0.5 -> moderate momentum
            * prevDeltaWeight;
        neuron._outputWeights[_myIdx].deltaWeight = currDeltaWeight;
        neuron._outputWeights[_myIdx].weight += currDeltaWeight;

    }
}
double Neuron::sumDOW(const Layer &nextlayer) const {
    double sum = 0.0;
    // Sum our contributiions of the errors at nodes we feed
    for (unsigned n = 0; n < nextlayer.size() - 1; ++n) {
        sum += _outputWeights[n].weight * nextlayer[n]._gradient;
    }
    return sum;
}
void Neuron::calcHiddenGradients(const Layer &nextLayer) {
    double dow = sumDOW(nextLayer);
    _gradient = dow * Neuron::transferFunctionDerivative(_outputVal);
}
void Neuron::calcOutputGradients(double targetVal) {
    double delta = targetVal - _outputVal;
    _gradient = delta * Neuron::transferFunctionDerivative(_outputVal);
}
double Neuron::transferFunction(double x) {
    // using a tanh x: could use a step, ramp,sigmond, etc
    // needs t be range [-1 ti 1]
    return tanh(x);
}
double Neuron::transferFunctionDerivative(double x) {
    // using a tanh x: could use a step, ramp,sigmond, etc
    // needs t be range [-1 ti 1]
    return 1 - x * x;
}
void Neuron::feedForward(const Layer &prevLayer) {
    double sum = 0.0;

    for (unsigned n = 0; n < prevLayer.size(); ++n) {
        sum += prevLayer[n].getOutputVal() *
            prevLayer[n]._outputWeights[_myIdx].weight;
    }
    _outputVal = Neuron::transferFunction(sum);
}

Neuron::Neuron(unsigned numOutputs, unsigned myIdx) {
    for (unsigned c = 0; c < numOutputs; ++c) {
        _outputWeights.push_back(Connection());
        _outputWeights.back().weight = randomWeight();
    }
    _myIdx = myIdx;
}



//********** class Net ********** //
class Net {

public:
    Net(const vector<unsigned> &topology);
    void feedForward(const vector<double> &inputVals);
    void backProp(const vector<double> &targetVals);
    void getResults(vector<double> &resultsVals) const;
    double getRecentAverageError(void) const { return _recentAverageError; }
private:
    vector<Layer> _layers; // _layers[layerNum][neuronNum]
    double _error;
    double _recentAverageError;
    static double _recentAverageSmoothFactor;

};

double Net::_recentAverageSmoothFactor = 100.0;


void Net::getResults(vector<double>& resultVals) const {
    resultVals.clear();
    for (unsigned n = 0; n < _layers.back().size() - 1; ++n) {
        resultVals.push_back(_layers.back()[n].getOutputVal());
    }
}

void Net::backProp(const vector<double>& targetVals) {
    // calc overal net err (RMS of output neuron errs)

    Layer& outputLayer = _layers.back();
    _error = 0.0;

    for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
        double delta = targetVals[n] - outputLayer[n].getOutputVal();
        _error += delta * delta;

    }
    _error /= outputLayer.size() - 1;
    _error = sqrt(_error); // RMS

    // Implement a recent average measurement
    
    _recentAverageError = (_recentAverageError * _recentAverageSmoothFactor + _error) / (_recentAverageSmoothFactor + 1.0);

    // calc output layer gradient

    for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
        outputLayer[n].calcOutputGradients(targetVals[n]);
    }

    // calc grad on hidden layers

    for (unsigned layerNum = _layers.size() - 2; layerNum > 0; --layerNum) {
        Layer &hiddenLayer = _layers[layerNum];
        Layer &nextLayer = _layers[layerNum + 1];

        for (unsigned n = 0; n < hiddenLayer.size(); ++n) {
            hiddenLayer[n].calcHiddenGradients(nextLayer);
        }
    }

    // for all layers from outputs to first hidden layer, update connection weight

    for (unsigned layerNum = _layers.size() - 1; layerNum > 0; --layerNum) {
        Layer &layer = _layers[layerNum];
        Layer &prevLayer = _layers[layerNum - 1];

        for (unsigned n = 0; n < layer.size() - 1; ++n) {
            layer[n].updateInputWeights(prevLayer);
        }
    }


}

void Net::feedForward(const vector<double>& inputVals) {
    assert(inputVals.size() == _layers[0].size() - 1);
    for (unsigned i = 0; i < inputVals.size(); ++i) {
        _layers[0][i].setOutputVal(inputVals[i]);
    }
    // forward propagation
    for (unsigned layerNum = 1; layerNum < _layers.size(); ++layerNum) {
        Layer& prevLayer = _layers[layerNum - 1];
        for (unsigned n = 0; n < _layers[layerNum].size() - 1; ++n) {
            _layers[layerNum][n].feedForward(prevLayer);
        }
    } 
}

Net::Net(const vector<unsigned>& topology) {
    unsigned numLayers = topology.size();
    for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum) {
        _layers.push_back(Layer());
        unsigned numOutputs = (layerNum == topology.size() - 1) ? 0 : topology[layerNum + 1];

        for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum) { // one more for bias
            _layers.back().push_back(Neuron(numOutputs,neuronNum)); //
            cout << "made a neuron" << endl;
        }
        // force bias to 1.0
        _layers.back().back().setOutputVal(1.0);
    }
}

void showVectorVals(string label, vector<double>& v)
{
    cout << label << " ";
    for (unsigned i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }

    cout << endl;
}

string printVectorVals(string label, vector<double>& v) {
    string res = "";
    res += label + " ";
    for (unsigned i = 0; i < v.size(); ++i) {
        res += to_string(v[i]) + " ";
    }

    res += "\n";
    return res;
}

void saveStringToFile(const std::string& filename, const std::string& content) {
    std::ofstream outputFile(filename);

    if (outputFile.is_open()) {
        outputFile << content;
        outputFile.close();
        std::cout << "String saved to " << filename << std::endl;
    }
    else {
        std::cerr << "Error: Unable to open the file for writing." << std::endl;
    }
}


int main()
{
    string filename = "FlipFlop";
    TrainingData trainData("test/" + filename + ".txt");
    string allData = "";
    vector<unsigned> topology;
    trainData.getTopology(topology);
    Net myNet(topology);
    vector<double> inputVals, targetVals, resultVals;
    int trainingPass = 0;

    while (!trainData.isEof()) {
        trainingPass++;
        cout << "\n" << "Pass" << trainingPass << "\n";
        allData += "\nPass:" + trainingPass;
        allData += '\n';

        if (trainData.getNextInputs(inputVals) != topology[0]) {
            break;
        }
        showVectorVals(" : Inputs:", inputVals);
        allData += printVectorVals(" : Inputs:", inputVals);

        myNet.feedForward(inputVals);

        myNet.getResults(resultVals);
        showVectorVals("Outputs:", resultVals);
        allData += printVectorVals("Outputs:", resultVals);

        trainData.getTargetOutputs(targetVals);
        showVectorVals("Targets:", targetVals);
        allData += printVectorVals("Targets:", targetVals);

        assert(targetVals.size() == topology.back());

        myNet.backProp(targetVals);

        cout << "Not recent average error: " << myNet.getRecentAverageError() << "\n";
        allData += "Not recent average error: " + to_string(myNet.getRecentAverageError()) + "\n";
    }
    saveStringToFile("result/" + filename + ".txt", allData);
    cout << "\n" << "Done" << endl;

}