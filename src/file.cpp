#include <fstream>
#include <sstream>

#include "file.h"
#include "pathfinder.h"


namespace File {
    bool LoadNodes(std::string filename, int& line_count, int& node_count, int& connection_count) {
        std::ifstream file(filename);

        if (!file.good()) {
            return false;
        }

        std::string buffer;
        int type = 0, id = 0, id2 = 0, direction = 0, ignore = 0;
        float x = 0.0f, y = 0.0f, z = 0.0f;

        while (std::getline(file, buffer)) {
            std::istringstream input(buffer);

            input >> type;

            switch (type) {
                case 0: {
                    input >> x >> y >> z >> ignore >> id;

                    if (Pathfinder::AddNode(id, x, y, z)) {
                        node_count++;
                    }

                    break;
                }

                case 1: {
                    input >> id >> id2 >> direction;

                    if (direction != 2 && Pathfinder::AddConnection(id, id2)) {
                        connection_count++;
                    }

                    break;
                }
            }

            line_count++;
        }

        file.close();
        return true;
    }
};