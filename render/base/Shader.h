#ifndef ENGINE_SHADER_H
#define ENGINE_SHADER_H

#include "../../includes.h"

class Shader {
private:
    int programId, vertexShaderId, fragmentShaderId;
    char errorBuffer[512] = "";
    int loadShaderFile(const std::string& filePath, GLenum shaderType);
protected:
    int getUniformLocation(std::string uniformName);
    void loadUniformFloat(int location, float value);
    void loadUniformVector(int location, vec3 vector);
    void loadUniformBool(int location, bool value);
    void loadUniformMatrix(int location, glm::mat4 matrix);

public:
    Shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
    void start();
    void stop();
    void clean();

    virtual void getUniformLocations() = 0;
};


#endif //ENGINE_SHADER_H
