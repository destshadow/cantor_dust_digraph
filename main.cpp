//
// Created by destshadow on 20/10/23.
//
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <string.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#define MAP_SIZE 256

static size_t map[MAP_SIZE][MAP_SIZE] = {0};
static uint32_t pixel[MAP_SIZE][MAP_SIZE] = {0};

std::string ReadBinaryFile(const std::string& path){

    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Impossibile aprire il file '" << path << "'." << std::endl;
        return "";
    }

    // Leggere il contenuto del file
    std::string data;
    while (!file.eof()) {
        char byte;
        file.read(&byte, 1);
        data.push_back(byte);
    }
    // Chiudere il file
    file.close();

    return data;
}

void CreateVisul(const std::string &data, const std::string& path){

    std::stringstream ss;
    ss << path;
    ss << ".out.png";

    std::string str = ss.str();

    const char* outfile = str.c_str();

    if(data.size() > 0){
        for(size_t i = 0; i < data.size()-1; ++i){
            uint8_t a = data[i];
            uint8_t b = data[i+1];
            //0xFFFFFFFF -> bianco
            map[a][b] += 1;
        }
    }

    float max = 0;

    for(int i = 0; i < MAP_SIZE; ++i){
        for(int j = 0; j < MAP_SIZE; ++j){
            float x = 0.0f;
            if(map[i][j] > 0){
                x = logf(map[i][j]);
            }
            if(x > max){
                max = x;
            }
        }
    }

    //normalize
    for(int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            float norm = logf(map[i][j]) / max;
            uint32_t brightness = norm * 255;
            //little endian channles
            //0xAABBGGRR
            pixel[i][j] = 0xFF000000 | brightness | (brightness << 8) | (brightness << 16);
        }
    }


    if(!stbi_write_png( outfile, MAP_SIZE, MAP_SIZE, 4, pixel, MAP_SIZE * sizeof (uint32_t))){
        std::cerr<<"Errore nella creazione dell'immagine"<<std::endl;
    }else{
        std::cout <<"Immagine creata, nuovo nome: " << outfile << std::endl;
    }

}


int main(int argc, char** argv){

    char* path;
    std::ifstream file;
    std::string data;

    if(argc < 2){
        std::cout << "Errore nel passaggio dei file"<< std::endl;
        return -1;
    }


    path = *(argv + 1);
    std::cout <<"Apro il file "<< path<<std::endl;
    data = ReadBinaryFile(path);
    std::cout <<"Creao la nuova immagine in base all'input" <<std::endl;
    CreateVisul(data, path);

    return 0;
}