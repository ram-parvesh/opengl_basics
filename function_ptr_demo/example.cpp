#include<fstream>
#include<vector>
#include<string>

std::string LoadShaderAsString(const std::string & filename){
    //resulting shader program loaded as a single string
    std::string result ="";

    std::string line ="";
    std::ifstream myFile(filename.c_str());

    if(myFile.is_open()){
        while(std::getline(myFile, line)){
            result += line + "\n";
        }
        myFile.close();
    }

    return result;

}



std::string  VertexShaderSource = LoadShaderAsString("./shaders/vert.glsl");
std::string FragmentShaderSource = LoadShaderAsString("./shaders/fragemnt.glsl");
