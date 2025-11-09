#include"Header_Files/shaderClass.h"
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define getcwd _getcwd
#endif

// Gets the directory where the executable is located
std::string get_executable_dir()
{
#ifdef _WIN32
	char path[MAX_PATH];
	if (GetModuleFileNameA(NULL, path, MAX_PATH) != 0)
	{
		std::string exePath(path);
		size_t lastSlash = exePath.find_last_of("\\/");
		if (lastSlash != std::string::npos)
		{
			return exePath.substr(0, lastSlash + 1);
		}
	}
	return "";
#else
	// For non-Windows, return empty (use relative paths)
	return "";
#endif
}

// Reads a text file and outputs a string with everything in the text file
// Tries multiple paths to find the shader file
std::string get_file_contents(const char* filename)
{
	std::string exeDir = get_executable_dir();
	
	// Try multiple possible paths
	std::string paths[] = {
		filename,                                    // Try as-is
		std::string("Resource_Files/Shaders/") + filename,  // Try relative to current dir
		std::string("../Resource_Files/Shaders/") + filename, // Try one level up
		std::string("../../Resource_Files/Shaders/") + filename, // Try two levels up
		exeDir + filename,                          // Try relative to exe
		exeDir + "Resource_Files/Shaders/" + filename,  // Try exe dir + Resource_Files
		exeDir + "../Resource_Files/Shaders/" + filename, // Try exe dir parent + Resource_Files
	};
	
	const int numPaths = sizeof(paths) / sizeof(paths[0]);
	
	for (int i = 0; i < numPaths; i++)
	{
		std::ifstream in(paths[i], std::ios::binary);
		if (in)
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return(contents);
		}
	}
	
	// If all paths failed, throw error
	std::cerr << "ERROR: Failed to open shader file: " << filename << std::endl;
	std::cerr << "Executable directory: " << (exeDir.empty() ? "(unknown)" : exeDir) << std::endl;
	std::cerr << "Tried paths:" << std::endl;
	for (int i = 0; i < numPaths; i++)
	{
		std::cerr << "  - " << paths[i] << std::endl;
	}
	throw(errno);
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);

	// Create Shader Program Object and get its reference
	ID = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(ID);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

// Activates the Shader Program
void Shader::Activate()
{
	glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(ID);
}