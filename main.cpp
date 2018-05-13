#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <array>
#include <algorithm>
#include <iomanip>
#include <tuple>
#include <cmath>


#include "Node.h"

bool isNumber(const std::vector<std::vector<std::string> >& s, const unsigned int column){
    try{
        for(unsigned int i=0; i<s.size(); i++)
            std::stod(s[i][column]);
    }
    catch(...){
        return false;
    }

    return true;
}

template<typename T, typename S>
void shellSort(std::vector<S>& v, std::vector<std::vector<T> >& v2, std::vector<std::vector<S> >& v3, const unsigned int column){
    size_t size = v.size();
    size_t h = 1;

    while(h < size/2.25)
        h = h*2.25 + 1;

    while(h >= 1){
        for(size_t i = h; i < size; i++){
            for(size_t j = i; j>=h && v[j] < v[j-h]; j-=h){
                std::swap(v[j-h], v[j]);
                std::swap(v2[j-h], v2[j]);
                for(size_t k=0; k<v3.size(); k++){
                    std::swap(v3[k][j-h], v3[k][j]);
                }
            }
        }
        h/=2.25;
    }
}

double calculateEntropy(std::vector<std::vector<unsigned int> >& examples, std::vector<std::vector<unsigned int> >& allAttributes){
    double entropy = 0;
    std::vector<unsigned int> auxCounter;
    for(unsigned int i=0; i<allAttributes[allAttributes.size()-1].size(); i++)
        auxCounter.push_back(0);

    for(unsigned int i=0; i<examples[0].size(); i++)
        auxCounter[examples[examples.size()-1][i]]++;

    double tmp;
    for(unsigned int i=0; i<auxCounter.size(); i++){
        tmp = (double)auxCounter[i]/examples[0].size();
        if(tmp != 0)
            entropy += tmp * log2(tmp);
    }

    return -entropy;
}

double calculateRemainder(std::vector<std::vector<unsigned int> >& examples, unsigned int attribute, std::vector<std::vector<unsigned int> >& allAttributes){
    double remainder = 0;
    for(unsigned int i=0; i<allAttributes[attribute].size(); i++){
        std::vector<std::vector<unsigned int> > examplesVi;
        for(unsigned int j=0; j<examples.size(); j++){
            std::vector<unsigned int> aux;
            examplesVi.push_back(aux);
        }

        for(unsigned int j=0; j<examples[0].size(); j++)
            if(examples[attribute][j] == allAttributes[attribute][i])
                for(unsigned int k=0; k<examples.size(); k++)
                    examplesVi[k].push_back(examples[k][j]);

        if(examplesVi[0].size() > 0)
            remainder += ((double)examplesVi[0].size()/examples[0].size()) * calculateEntropy(examplesVi, allAttributes);
    }

    return remainder;
}

unsigned int chooseAttribute(std::vector<std::vector<unsigned int> >& examples, std::vector<unsigned int>& attributes, std::vector<std::vector<unsigned int> >& allAttributes){
    double entropy = calculateEntropy(examples, allAttributes);
    unsigned int bestAttribute = 0;
    double bestGain = entropy - calculateRemainder(examples, attributes[0], allAttributes);

    for(unsigned int i=1; i<attributes.size(); i++){
        double tmp = entropy - calculateRemainder(examples, attributes[i], allAttributes);
        if(tmp > bestGain){
            bestGain = tmp;
            bestAttribute = attributes[i];
        }
    }

    return bestAttribute;
}

std::tuple<std::vector<std::vector<unsigned int> >, std::vector<std::vector<unsigned int> >, std::vector<std::vector<std::string> > > discretizeAttributes(std::vector<std::vector<std::string> >& examples){

    //Change string to number and discretize numbers (http://id3alg.altervista.org/)
    std::vector<std::vector<unsigned int> > examplesSimplified;
    std::vector<std::vector<unsigned int> > allAttributes;
    std::vector<std::vector<std::string> > allAttributesStrings;
    
    for(unsigned int i=1; i<examples[0].size(); i++){
        std::vector<unsigned int> auxExamples;
        std::vector<unsigned int> auxAttributes;
        std::vector<std::string> auxAttributesStrings;
        
        //If field is made of numbers, then discretize the numbers
        if(isNumber(examples, i)){

            //Convert column to numbers
            for(unsigned int j=0; j<examples.size(); j++)
                auxExamples.push_back(std::stod(examples[j][i]));
            
            //https://www.youtube.com/watch?v=MK_dMsn4MqI
            //10.0 so that it converts to double
            double stepSize = auxExamples.size()/10.0;
            shellSort(auxExamples, examples, examplesSimplified, i);

            unsigned int start = 0;
            unsigned int last;
            for(unsigned int j=0; j<10; j++){
                last = (j+1)*stepSize;

                for(unsigned int k=start; k<last; k++)
                    auxExamples[k] = j;
                

                start = last;
                auxAttributes.push_back(j);
                auxAttributesStrings.push_back(std::to_string(j));
            }
        }
        //If the field is made of strings, then modify them to numbers
        else {
            std::vector<std::string> auxIndex;
            for(unsigned int j=0; j<examples.size(); j++){
                if(std::find(auxIndex.begin(), auxIndex.end(), examples[j][i]) != auxIndex.end())
                    auxExamples.push_back(std::distance(auxIndex.begin(), std::find(auxIndex.begin(), auxIndex.end(), examples[j][i])));
                else{
                    auxExamples.push_back(auxIndex.size());
                    auxAttributes.push_back(auxIndex.size());
                    auxAttributesStrings.push_back(examples[j][i]);
                    auxIndex.push_back(examples[j][i]);
                }
            }
        }

        examplesSimplified.push_back(auxExamples);
        allAttributes.push_back(auxAttributes);
        allAttributesStrings.push_back(auxAttributesStrings);
    }

    return std::make_tuple(examplesSimplified, allAttributes, allAttributesStrings);
}

std::vector<std::vector<std::string> > readCSVFile(char const *file){
    //Open examples file
    std::ifstream csvFile;
    csvFile.open(file, std::ifstream::in);



    //Load everything in to a vector
    std::string buffer;
    std::vector<std::vector<std::string> > examples;

    while(std::getline(csvFile, buffer)){
        std::vector<std::string> aux;
        size_t last = 0;
        size_t next = 0;
        while((next = buffer.find(",", last)) != std::string::npos){
            aux.push_back(buffer.substr(last, next-last));
            last = next + 1;
        }
        aux.push_back(buffer.substr(last));

        examples.push_back(aux);
    }

    return examples;
}

Node* ID3(std::vector<std::vector<unsigned int> >& examples, unsigned int targetAttribute, std::vector<unsigned int>& attributes, std::vector<std::vector<unsigned int> >& allAttributes){
    Node* root = new Node();
    root->setAttribute(targetAttribute);

    unsigned int index;
    unsigned int count = 0;
    for(index=1; index<examples[examples.size()-1].size(); index++){
        if(examples[examples.size()-1][index] != examples[examples.size()-1][index-1])
            break;
        count++;
    }

    if(index == examples[examples.size()-1].size()){
        root->setAttribute(examples[examples.size()-1][index-1]);
        root->setCount(count+1);
        root->setLeaf();
        return root;
    }

    if(attributes.size() == 0){
        std::vector<unsigned int> auxCounter;

        for(unsigned int i=0; i<examples[examples.size()-1].size(); i++)
            auxCounter.push_back(0);

        for(unsigned int j=0; j<examples[examples.size()-1].size(); j++)
            auxCounter[examples[examples.size()-1][j]]++;

        unsigned int max = 0;
        for(unsigned int i=1; i<auxCounter.size(); i++)
            if(auxCounter[i] > auxCounter[max])
                max = i;

        count = 0;
        for(unsigned int i=0; i<examples[examples.size()-1].size(); i++)
            if(examples[examples.size()-1][i] == max)
                count++;

        root->setAttribute(max);
        root->setCount(count);
        root->setLeaf();
        return root;
    }

    unsigned int bestAttribute = chooseAttribute(examples, attributes, allAttributes);
    root->setAttribute(bestAttribute);

    for(unsigned int i=0; i<allAttributes[bestAttribute].size(); i++){
        std::vector<std::vector<unsigned int> > examplesVi;
        for(unsigned int j=0; j<examples.size(); j++){
            std::vector<unsigned int> aux;
            examplesVi.push_back(aux);
        }

        for(unsigned int j=0; j<examples[bestAttribute].size(); j++)
            if(examples[bestAttribute][j] == allAttributes[bestAttribute][i])
                for(unsigned int k=0; k<examples.size(); k++)
                    examplesVi[k].push_back(examples[k][j]);

        if(examplesVi[0].size() == 0){
            std::vector<unsigned int> auxCounter;

            for(unsigned int j=0; j<allAttributes[allAttributes.size()-1].size(); j++)
                auxCounter.push_back(0);

            for(unsigned int j=0; j<examples[examples.size()-1].size(); j++)
                auxCounter[examples[examples.size()-1][j]]++;

            unsigned int max = 0;
            for(unsigned int j=1; j<auxCounter.size(); j++)
                if(auxCounter[j] > auxCounter[max])
                    max = j;

            Node* leaf = new Node();
            leaf->setAttribute(max);
            leaf->setCount(0);
            leaf->setLeaf();
            root->insertChild(leaf, allAttributes[bestAttribute][i]);
            continue;
        }

        std::vector<unsigned int> attributesMinus;
        for(unsigned int j=0; j<attributes.size(); j++)
            if(attributes[j] != bestAttribute)
                attributesMinus.push_back(attributes[j]);

        root->insertChild(ID3(examplesVi, bestAttribute, attributesMinus, allAttributes), allAttributes[bestAttribute][i]);
    }

    return root;
}

void printTree(Node* root, std::vector<std::string>& header, std::vector<std::vector<std::string> >& allAttributesStrings, unsigned int tabs){
    std::vector<Node *> children = root->getChildren();
    std::vector<unsigned int> childrenLabels = root->getChildrenLabels();

    for(unsigned int i=0; i<tabs; i++)
        std::cout << "\t";

    std::cout << "<" << header[root->getAttribute()+1] << ">" << std::endl;
    for(unsigned int i=0; i<children.size(); i++){
        for(unsigned int i=0; i<tabs+1; i++)
            std::cout << "\t";

        if(children[i]->isLeaf() == true){
            std::cout << allAttributesStrings[root->getAttribute()][childrenLabels[i]] << ": "
                    << allAttributesStrings[allAttributesStrings.size()-1][children[i]->getAttribute()] 
                    << " (" << children[i]->getCount() << ")" << std::endl;
        }
        else{
            std::cout << allAttributesStrings[root->getAttribute()][childrenLabels[i]] << ": " << std::endl;
            printTree(children[i], header, allAttributesStrings, tabs+2);
        }
    }
}

int main(int argc, char const *argv[]){

    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " [cvsFile]" << std::endl;
        return 1;
    }

    //Load everything in to a vector
    std::vector<std::vector<std::string> > examples = readCSVFile(argv[1]);

    std::vector<std::string> header = examples[0];
    examples.erase(examples.begin(), examples.begin()+1);

    //Discretize values
    std::tuple<std::vector<std::vector<unsigned int> >, std::vector<std::vector<unsigned int> >, std::vector<std::vector<std::string> > > tuple = discretizeAttributes(examples);
    std::vector<std::vector<unsigned int> > examplesSimplified = std::get<0>(tuple);
    std::vector<std::vector<unsigned int> > allAttributes = std::get<1>(tuple);
    std::vector<std::vector<std::string> > allAttributesStrings = std::get<2>(tuple);
    
    std::vector<unsigned int> attributes;
    for(unsigned int i=0; i<examplesSimplified.size()-1; i++)
        attributes.push_back(i);

    unsigned int bestAttribute = chooseAttribute(examplesSimplified, attributes, allAttributes);
    
    Node* root = ID3(examplesSimplified, bestAttribute, attributes, allAttributes);

    printTree(root, header, allAttributesStrings, 0);

    delete root;

    return 0;
}