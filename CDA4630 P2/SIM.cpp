//I have neither given nor recieved any unauthorized aid on this assignment.
#include <stdlib.h>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

void uncompressed_read(std::vector<std::string> &allEntries, std::map<std::string, std::pair<int, int>> &entryInfo){
    std::ifstream file1("original.txt");
    std::string line;
    int i = 0;
    if(file1.is_open()){
        while(getline(file1, line)){
            allEntries.push_back(line);
            std::pair<int, int> x;
            x.first = 0;
            x.second = i;
            entryInfo.emplace(line, x);
            i++;
        }
        file1.close();
    } else {
        std::cout << "Failed to open original.txt";
    }
}
std::string biConvert(int num){
    if(num == 0){
        return "0000";
    }
    std::string bits;
    while(num > 0){
        if(num % 2 == 0){
            bits.push_back(static_cast<char>(48));
        }
        if(num % 2 == 1){
            bits.push_back(static_cast<char>(49));
        }
        num /= 2;
    }
    std::reverse(bits.begin(), bits.end());
    return bits;
}
std::string bitMaskedBasedCompression(std::string entry, std::string dictionEntry, int index, int dicionIndex){
    for(int i = index + 4; i < 32; i++){
        if(entry[i] != dictionEntry[i]){
            std::string empti;
            empti.clear();
            return empti;
        }
    }
    std::string format = "010 --> ";
    std::string mm = biConvert(index);
    std::string diN = biConvert(dicionIndex);
    std::string bm = "1";
    while(mm.length() < 5){
        mm.insert(mm.begin(), '0');
    }
    int next = index + 1;
    while(next < (index + 4)){
        if(entry[next] == dictionEntry[next]){
            bm.insert(bm.end(), '0');
        } else if(entry[next] != dictionEntry[next]){
            bm.insert(bm.end(), '1');
        }
        next++;
    } 
    if(bm.length() < 4){
        bm.insert(bm.begin(), '0');
    } 
    if(diN.length() < 4){
        diN.insert(diN.begin(), '0');
    } 
    std::string compressed = format + mm + " " + bm + " " + diN;
    return compressed;
}
std::string oneBitMismatch(std::vector<std::pair<int, int>> vec){
    std::string format = "011 --> "; 
    std::string misLo = biConvert(vec[0].second);
    std::string dictionIndex = biConvert(vec[0].first);
    while(misLo.length() < 5){
        misLo.insert(misLo.begin(), '0');
    }
    while(dictionIndex.length() < 4){
        dictionIndex.insert(dictionIndex.begin(), '0');
    }
    std::string compressed = format + misLo + dictionIndex;
    return compressed;
}
std::string twoBitMismatchConsecutive(std::vector<std::pair<int, int>> vec){
    std::string format = "100 --> ";
    std::string mm = biConvert(vec[0].second);
    std::string dicitonIndex = biConvert(vec[0].first);
    while(mm.length() < 5){
        mm.insert(mm.begin(), '0');
    }
    while(dicitonIndex.length() < 4){
        dicitonIndex.insert(dicitonIndex.begin(), '0');
    }
    std::string compressed = format + mm + dicitonIndex;
    return compressed;    
}
std::string fourBitMismatch(std::vector<std::pair<int, int>> vec){
    std::string format = "101 --> ";
    std::string mm = biConvert(vec[0].second);
    std::string dictionIndex = biConvert(vec[0].first);
    if(mm.length() < 5){
        mm.insert(mm.begin(), '0');
    }
    if(dictionIndex.length() < 4){
        dictionIndex.insert(dictionIndex.begin(), '0');
    }
    std::string compressed = format + mm + dictionIndex;
    return compressed;
}
std::string twoBitMismatchAnywhere(std::vector<std::pair<int, int>> vec){
    std::string format = "110 --> ";
    std::string firstMM = biConvert(vec[0].second);
    std::string secondMM = biConvert(vec[1].second);
    std::string dicitonIndex = biConvert(vec[0].first);
    while(firstMM.length() < 5){
        firstMM.insert(firstMM.begin(), '0');
    }
    while(secondMM.length() < 5){
        secondMM.insert(secondMM.begin(), '0');
    }
    while(dicitonIndex.length() < 4){
        dicitonIndex.insert(dicitonIndex.begin(), '0');
    }
    std::string compressed = format + firstMM + secondMM + dicitonIndex;
    return compressed;
}
std::string directMatching(int i){
    std::string format = "111 --> ";
    std::string index = biConvert(i);
    while(index.length() < 4){
        index.insert(index.begin(), '0');
    }
    std::string compressed = format + index;
    return compressed;
}
int main(){
    std::vector<std::string> allEntries;
    std::map<std::string, std::pair<int, int>> entryInfo;
    uncompressed_read(allEntries, entryInfo);
    for(auto entry : allEntries){
        entryInfo[entry].first++;
    }
    std::vector<std::pair<std::string, std::pair<int, int>>> dictionary(entryInfo.begin(), entryInfo.end());
    std::sort(dictionary.begin(), dictionary.end(), [](const std::pair<std::string, std::pair<int, int>>& a, const std::pair<std::string, std::pair<int, int>>& b){
        return a.second.first > b.second.first;
    });
    for(int i = 0; i < dictionary.size(); ){
        int j = i + 1;
        while(i < dictionary.size() - 1 && dictionary[i].second.first == dictionary[j].second.first){
            j++;
        }
        std::sort(dictionary.begin() + i, dictionary.begin() + j, [](const std::pair<std::string, std::pair<int, int>>& a, const std::pair<std::string, std::pair<int, int>>& b){
            return a.second.second < b.second.second;
        });
        i = j;
    }
    //runLength
    std::cout << allEntries.size() << std::endl;
    for(int i = 0; i < allEntries.size() - 1; ){
        if(allEntries[i] == allEntries[i + 1]){
            if(i < allEntries.size() - 2 && allEntries[i] == allEntries[i + 2]){
                allEntries.erase(allEntries.begin() + i);
            } else {
                i += 2;
            }
        } else {
            i++;
        }
    }

    std::vector<std::pair<int, int>> mismatch_indices;
    std::vector<std::pair<std::string, int>> compressionLengths;
    std::vector<std::string> finalCompression;
    for(int i = 0; i < allEntries.size(); i++){
        compressionLengths.clear();
        for(int j = 0; j < dictionary.size(); j++){
            mismatch_indices.clear();
            //Direct matching
            if(allEntries[i] == dictionary[j].first){
                std::pair<std::string, int> dmPair(directMatching(j), directMatching(j).length());
                compressionLengths.push_back(dmPair);
            } else {
                for(int k = 0; k < 32; k++){
                    if(allEntries[i][k] != dictionary[j].first[k]){
                        std::pair<int, int> newPair(j, k);
                        mismatch_indices.push_back(newPair);
                        if(k < 29){
                            std::pair<std::string, int> bmBCPair(bitMaskedBasedCompression(allEntries[i], dictionary[j].first, k, j), bitMaskedBasedCompression(allEntries[i], dictionary[j].first, k, j).length());
                            if(!(bitMaskedBasedCompression(allEntries[i], dictionary[j].first, k, j).empty())){
                                compressionLengths.push_back(bmBCPair);
                            }
                        }
                    }
                }
                //1-bit mismatch 
                if(mismatch_indices.size() == 1){
                    std::pair<std::string, int> oneBMM(oneBitMismatch(mismatch_indices), oneBitMismatch(mismatch_indices).length());
                    compressionLengths.push_back(oneBMM);
                }
                if(mismatch_indices.size() == 2){
                    //2-bit consecutive mismatch
                    if(mismatch_indices[1].second == (mismatch_indices[0].second + 1)){
                        std::pair<std::string, int> twoBCMM(twoBitMismatchConsecutive(mismatch_indices), twoBitMismatchConsecutive(mismatch_indices).length());
                        compressionLengths.push_back(twoBCMM);
                    }
                    //2-bit anywhere mismatch
                    else{
                        std::pair<std::string, int> twoBMMA(twoBitMismatchAnywhere(mismatch_indices), twoBitMismatchAnywhere(mismatch_indices).length());
                        compressionLengths.push_back(twoBMMA);
                    }
                }
                //4-bit consecutive mismatch
                if((mismatch_indices.size() == 4) && (mismatch_indices[3].second == (mismatch_indices[2].second + 1)) && 
                    (mismatch_indices[2].second == (mismatch_indices[1].second + 1)) && (mismatch_indices[1].second == (mismatch_indices[0].second + 1))){
                    std::pair<std::string, int> fourBCMM(fourBitMismatch(mismatch_indices), fourBitMismatch(mismatch_indices).length());
                    compressionLengths.push_back(fourBCMM);
                }
                //Original Binary
                std::string format = "000";
                std::string compressed = format + allEntries[i];
                std::pair<std::string, int> OB(compressed, compressed.length());
                compressionLengths.push_back(OB);

                //std::sort(compressionLengths.begin(), compressionLengths.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b){
                    //return a.second < b.second;
                //});
            }
        }
        //for(auto x : compressionLengths){
            std::cout << allEntries[i] << " " << compressionLengths[0].first << std::endl;
        //}
        //std::cout << allEntries[i] << " " << compressionLengths[0].first << std::endl;
        // std::cout << compressionLengths.size() << std::endl;
    }
    return 0;
}