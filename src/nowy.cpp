#include <iostream>
#include <fstream>
#include <list>

void loadVertices() {
  std::list<float> vertices;
  std::list<unsigned int> indices;

  std::ifstream file("/home/js/cpp/grafika/src/raz.obj");
  std::string line;
  if (file.is_open()) {
    while (std::getline(file, line)) {
      if (line[0] == 'v') {
        std::string::size_type s;
        line = line.substr(1, line.length()-1);
        double d = std::stof(line, &s);
        vertices.push_back(d);
        line = line.substr(s);
        d = std::stof(line, &s);
        vertices.push_back(d);
        d = std::stof(line, &s);
        line = line.substr(s);
        d = std::stof(line, &s);
        vertices.push_back(d);
      } else if (line[0] == 'f') {
        std::string::size_type s;
        line = line.substr(1, line.length()-1);
        unsigned int i = std::stoul(line, &s);
        indices.push_back(--i);
        line = line.substr(s);
        i = std::stoul(line, &s);
        indices.push_back(--i);
        line = line.substr(s);
        i = std::stoul(line, &s);
        indices.push_back(--i);
        line = line.substr(s);
      }
    }
    file.close();
  } else {
    std::cout << "loadVertices ERROR" << std::endl;
  }
  float* ver = new float[vertices.size()];
  unsigned int* ind = new unsigned int[indices.size()];
  int i = 0;
  std::cout << "vertices: " << std::endl;
  for (float const &k : vertices) {
    ver[i++] = k;
    std::cout << k << " ";
  }
  i = 0;
  std::cout << std::endl;
  std::cout << "indices: " << std::endl;
  for (int const &k : indices) {
    ind[i++] = k;
    std::cout << k << " ";
  }
  std::cout << std::endl;
  std::cout << "vertex size: " << vertices.size() << std::endl;
  std::cout << "indices size: " << indices.size() << std::endl;
  
  unsigned int indices_size = indices.size();

  delete [] ver;
  delete [] ind;

}

int main() {
  loadVertices();
  return 0;
}
