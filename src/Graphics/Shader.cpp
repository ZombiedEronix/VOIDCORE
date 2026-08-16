#include "Shader.h"

Shader::Shader(const char *VertexFile, const char *FragmentChar)
{
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#if defined _DEBUG
	Debug::Log("---------------------------------");
	Debug::Log("Loading shader from files: ", VertexFile, " and ", FragmentChar);
#endif
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	std::string VertexCode = get_file_contents(VertexFile);
	std::string FragmentCode = get_file_contents(FragmentChar);
	const char *VertexSource = VertexCode.c_str();
	const char *FragmentSource = FragmentCode.c_str();
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &VertexSource, NULL);
	glCompileShader(vertexShader);
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	GLint success;
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FragmentSource, NULL);
	glCompileShader(fragmentShader);

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

std::string get_file_contents(const char *filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return (contents);
	}
#if defined _DEBUG
	else
	{
		Debug::Log("Failed to load shader file: ", filename);
	}
#endif
	return "";
}

int Shader::GetUniformLocation(const std::string& name)
{
	auto it = uniformCache.find(name);
	if(it != uniformCache.end()) return it->second;

	int loc = glGetUniformLocation(ID, name.c_str());
	uniformCache[name] = loc;
	return loc;
}

void Shader::SetInt(const std::string &name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string &name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}


void Shader::Delete()
{
	glDeleteProgram(ID);
}

void Shader::Load()
{
	glUseProgram(ID);
}
