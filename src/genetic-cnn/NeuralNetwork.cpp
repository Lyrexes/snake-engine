#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(int input_nodes, std::vector<int> hiddenLayer, int output, float minRan, float maxRan){
	this->input_nodes = input_nodes;
	this->output_nodes = output;
	this->layer_count = hiddenLayer.size() + 1;
	this->learning_rate = LEARNING_RATE;
	this->minRan = minRan;
	this->maxRan = maxRan;
	this->hiddenLayer = hiddenLayer;
	this->sigmoid = [](double &x) { x = 1 / (1 + exp(-x)); };
	this->dsigmoid = [](double &x) {//return sigmoid(x) * (1 - sigmoid(x));
		x = x * (1 - x);
	};
	this->random = [minRan, maxRan](double& x) {
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_real_distribution<double> dist(minRan, maxRan);
		x =  dist(rng);
	};
	this->initWeights();
}

NeuralNetwork::NeuralNetwork() {}

NeuralNetwork::~NeuralNetwork() {
}

void NeuralNetwork::initRandValsdouble(arma::mat& matrix){
	matrix.for_each(random);
}

void NeuralNetwork::initRandAddGauss(arma::mat& matrix) {
	arma::mat t = matrix;
	t = arma::randg<arma::mat>(matrix.n_rows, matrix.n_cols);
	matrix += t;
}

void NeuralNetwork::train(std::vector<double> input, std::vector<double> target) {
	std::vector<double> output = this->feedForward(input);
	this->backpropagation(output, target);
	inputPerLayer.clear();
	outputPerLayer.clear();
}


std::vector<double> NeuralNetwork::feedForward(std::vector<double> input) {
	if (input.size() != input_nodes) {
		std::cerr << "Input-Error: Input amount does not match with the constructed input_nodes!";
		exit(EXIT_FAILURE);
	}
	arma::mat inputMat = arma::mat(input);
	arma::mat output;
	for (int layer = 0; layer < layer_count; layer++) { //Iterates through the layers
		inputPerLayer.push_back(inputMat);
		output = weights[layer] * inputMat;//Dot product Weights * Input
		output += biases[layer];           //Matrix addition (weights * Input) + Bias
		output.for_each(sigmoid);		   //Activation function
		outputPerLayer.push_back(output);
		inputMat = output;				   // Next Layer Output becomes the input
	}
	return matToVec1D(output);
}

std::vector<arma::mat>& NeuralNetwork::getWeights() {
	return weights;
}

std::vector<arma::mat>& NeuralNetwork::getBiases() {
	return biases;
}

void NeuralNetwork::setWeights(std::vector<arma::mat> newWeights) {
	weights = newWeights;
}

void NeuralNetwork::setBiases(std::vector<arma::mat> newBiases) {
	biases = newBiases;
}

void NeuralNetwork::backpropagation(std::vector<double> output, std::vector<double> target) {
	if (output.size() != target.size()) {
		std::cerr << "Error: Output and Target size does not match";
		exit(EXIT_FAILURE);
	}
	//ERROR = TARGETS - OUTPUTS
	std::vector< arma::Mat<double> >errors;
	errors.resize(layer_count);
	arma::Mat<double> errorOutput = arma::Mat<double>(output);
	arma::Mat<double> targetMat = arma::Mat<double>(target);
	arma::Mat<double> outputMat = arma::Mat<double>(output);
	errorOutput = targetMat - errorOutput;
	double weightSum = 0;
	arma::Mat<double> transposedWeight;
	for (int layer = layer_count - CONST_ONE; - CONST_ONE < layer; layer--) {//Backtrace the error	
		if (layer == layer_count - CONST_ONE) //error of the output 
			errors[layer] = errorOutput;
		else {
			transposedWeight = weights[layer + CONST_ONE].t(); 
			errors[layer] = transposedWeight * errorOutput;
			errorOutput = errors[layer]; // Matrix of Errors of one layer
		}
	}
	arma::Mat<double> gradients;
	arma::Mat<double> inputsPerLyTrans;
	arma::Mat<double> deltaBias;
	arma::Mat<double> deltaWeight;
	for (int layer = layer_count - CONST_ONE; -CONST_ONE < layer; layer--) {//Calculate thr gradient descent
		gradients = outputPerLayer[layer];//output of this current neuron as a start value
		gradients.for_each(dsigmoid);	  //pass the outputs in the derivative of sigma   
		gradients %= errors[layer];		  //Multiply it element wise with the error of this neuron  
		deltaBias = gradients; 
		deltaBias *= learning_rate;       //Multiply the learning rate 
		biases[layer] += deltaBias;		  //change the bias by delta Bias	 
		inputsPerLyTrans = inputPerLayer[layer].t(); //transpose the inputs 
		deltaWeight = gradients * inputsPerLyTrans;  //multiplay the transposed input
		deltaWeight *= learning_rate;                //Multiply the learning rate 
		weights[layer] += deltaWeight;				 // change the weights by deltaWeight (gradient descent)      
	}
}

void NeuralNetwork::initWeights() {
	source_count.resize(layer_count);
	destination_count.resize(layer_count);
	weights.resize(layer_count);
	biases.resize(layer_count);
	source_count[0] = input_nodes;
	destination_count[0] = hiddenLayer[0];
	for (int i = 1; i < layer_count; i++) {

		if (i == layer_count - CONST_ONE) {
			source_count[i] = hiddenLayer[i - CONST_ONE];
			destination_count[i] = output_nodes;
		}
		else {
			destination_count[i] = hiddenLayer[i];
			source_count[i] = hiddenLayer[i - CONST_ONE];
		}
	}
	for (int i = 0; i < layer_count; i++) {
		weights[i] = arma::Mat<double>(destination_count[i], source_count[i]);
		initRandValsdouble(weights[i]);
		biases[i] = arma::Mat<double>(destination_count[i], 1); // Always 1 dimensional
		initRandValsdouble(biases[i]);
	}
}

void NeuralNetwork::loading() {
	weights.clear();
	biases.clear();
	std::ifstream dataFileOutput("dataFile.json", std::ifstream::binary);
	dataFileOutput.open("dataFile.json");
	Json::Value root;
	Json::Value arrayWeight(Json::arrayValue);
	Json::Value arrayBias(Json::arrayValue);
	try {
		dataFileOutput >> root;
	}
	catch (const std::exception& e) {
		std::cerr << "could not find .json file !" << std::endl;
	}
	int weightSize = root["WeightCount"].asInt();
	int biasSize = root["BiasCount"].asInt();
	weights.resize(weightSize);
	biases.resize(biasSize);
	arma::Row<double> rowsB;
	arma::Row<double> rowsW;
	int n_rowsW = 0;
	int n_colsW = 0;
	int n_rowsB = 0;
	int n_colsB = 0;
	for (int index = 0; index < layer_count; index++) {
		n_rowsW = root["Matrix"]["weights"][index]["rowCount"].asInt();
		n_colsW = root["Matrix"]["weights"][index]["colCount"].asInt();
		n_rowsB = root["Matrix"]["biases"][index]["rowCount"].asInt();
		n_colsB = root["Matrix"]["biases"][index]["colCount"].asInt();
		rowsB.set_size(n_colsB);
		rowsW.set_size(n_colsW);
		arrayWeight = root["Matrix"]["weights"][index]["data"];
		arrayBias = root["Matrix"]["biases"][index]["data"];
		for (int rowB = 0; rowB < n_rowsB; rowB++) {
			int offset = rowB * n_colsB;
			for (int colB = 0; colB < n_colsB; colB++) {
				rowsB[colB] = arrayBias[index][offset + colB].asDouble();
			}
			biases[index].insert_rows(rowB, rowsB);
		}
		for (int rowW = 0; rowW < n_rowsW; rowW++) {
			int offset = rowW * n_colsW;
			for (int colW = 0; colW < n_colsW; colW++)
				rowsW[colW] = arrayWeight[index][offset + colW].asDouble();
			weights[index].insert_rows(rowW, rowsW);
		}
		rowsB.reset();
		rowsW.reset();
	}
}

void NeuralNetwork::saving() {
	Json::StreamWriterBuilder writerBuilder;
	std::unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());
	try {
		std::remove("dataFile.json");
	}
	catch (const std::exception&) {}
	std::ofstream dataFileInput("dataFile.json", std::ofstream::in);
	dataFileInput.open("dataFile.json");
	int index = 0;
	Json::Value wts(Json::arrayValue);
	Json::Value bs(Json::arrayValue);
	Json::Value root;
	arma::Row<double> rowsW;
	arma::Row<double> rowsB;
	root["WeightCount"] = weights.size();
	root["BiasCount"] = biases.size();
	for (arma::Mat<double> matrix : this->weights) {
		root["Matrix"]["weights"][index]["rowCount"] = matrix.n_rows;
		root["Matrix"]["weights"][index]["colCount"] = matrix.n_cols;
		root["Matrix"]["biases"][index]["rowCount"] = biases[index].n_rows;
		root["Matrix"]["biases"][index]["colCount"] = biases[index].n_cols;
		for (int rowB = 0; rowB < biases[index].n_rows; rowB++) {
			rowsB = arma::Row<double>(biases[index].row(rowB));
			for (double val : rowsB)
				bs[index].append(val);
		}
		for (int row = 0; row < matrix.n_rows; row++) {
			rowsW = arma::Row<double>(matrix.row(row));
			for (double val : rowsW)
				wts[index].append(val);
		}
		root["Matrix"]["weights"][index]["data"] = wts;
		root["Matrix"]["biases"][index]["data"] = bs;
		bs.clear();
		wts.clear();
		index++;
	}
	writerBuilder["commentStyle"] = "None";
	writerBuilder["indentation"] = "   ";
	writer->write(root, &dataFileInput);
	dataFileInput.close();
}

std::vector<double> NeuralNetwork::matToVec1D(arma::mat& A) {

	 std::vector<double> V(A.n_rows);
	for (size_t i = 0; i < A.n_rows; ++i) {
		V[i] = arma::conv_to< double >::from(A.row(i));
	}
	return V;
}
