#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <array>
#include <algorithm>
#include <iomanip>

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

std::vector<std::vector<unsigned int> > discretizeAttributes(std::vector<std::vector<std::string> >& examples){

    //Change string to number and discretize numbers (http://id3alg.altervista.org/)
    std::vector<std::vector<unsigned int> > examplesSimplified;
    
    for(unsigned int i=1; i<examples[0].size(); i++){
        std::vector<unsigned int> aux;
        
        //If field is made of numbers, then discretize the numbers
        if(isNumber(examples, i)){

            //Convert column to numbers
            for(unsigned int j=0; j<examples.size(); j++)
                aux.push_back(std::stod(examples[j][i]));
            
            //https://www.youtube.com/watch?v=MK_dMsn4MqI
            //10.0 so that it converts to double
            double stepSize = aux.size()/10.0;
            shellSort(aux, examples, examplesSimplified, i);

            unsigned int start = 0;
            unsigned int last;
            for(unsigned int j=0; j<10; j++){
                last = (j+1)*stepSize;

                for(unsigned int k=start; k<last; k++)
                    aux[k] = j;
                
                start = last;
            }
        }
        //If the field is made of strings, then modify them to numbers
        else {
            std::vector<std::string> auxIndex;
            for(unsigned int j=0; j<examples.size(); j++){
                if(std::find(auxIndex.begin(), auxIndex.end(), examples[j][i]) != auxIndex.end())
                    aux.push_back(std::distance(auxIndex.begin(), std::find(auxIndex.begin(), auxIndex.end(), examples[j][i])));
                else{
                    aux.push_back(auxIndex.size());
                    auxIndex.push_back(examples[j][i]);
                }
            }
        }

        examplesSimplified.push_back(aux);
    }

    return examplesSimplified;
}

std::vector<std::vector<std::string> > readCSVFile(char const *file){
    //Open examples file
    std::ifstream csvFile;
    csvFile.open(file, std::ifstream::in);

    

    //Read header to see which attributes exist
    std::string header;
    std::getline(csvFile, header);


    
    //Add atributes to vec by separating the string with delimiter ','
    std::vector<std::string> attributes;
    size_t last = 0;
    size_t next = 0;
    while((next = header.find(",", last)) != std::string::npos){
        attributes.push_back(header.substr(last, next-last));
        last = next + 1;
    }
    attributes.push_back(header.substr(last));

    

    //Load everything in to a vector
    std::vector<std::vector<std::string> > examples;

    while(std::getline(csvFile, header)){
        std::vector<std::string> aux;
        size_t last = 0;
        size_t next = 0;
        while((next = header.find(",", last)) != std::string::npos){
            aux.push_back(header.substr(last, next-last));
            last = next + 1;
        }
        aux.push_back(header.substr(last));

        examples.push_back(aux);
    }

    return examples;
}

int main(int argc, char const *argv[]){

    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " [cvsFile]" << std::endl;
        return 1;
    }

    //Load everything in to a vector
    std::vector<std::vector<std::string> > examples = readCSVFile(argv[1]);

    //Discretize values
    std::vector<std::vector<unsigned int> > examplesSimplified = discretizeAttributes(examples);


    for(unsigned int i=0; i<examples.size(); i++){
        for(unsigned int j=0; j<examples[i].size(); j++)
            std::cout << std::setw(10) << examples[i][j];
        std::cout << std::endl;
    }

    for(unsigned int i=0; i<examplesSimplified[0].size(); i++){
        for(unsigned int j=0; j<examplesSimplified.size(); j++)
            std::cout << std::setw(5) << examplesSimplified[j][i];
        std::cout << std::endl;
    }

    return 0;
}