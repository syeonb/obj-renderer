#include "objloader.hpp"

// Here is a short list of features a real function would provide :
// - Loading from memory, stream, etc

bool loadOBJ(
        const char * path,
        std::vector<GLfloat> & vertices,
        std::vector<GLuint> & indices,
        std::vector<GLfloat> & out_texcoord
        )
    {
    printf("starting to load file\n");
	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? \n");
        perror("Error");
		getchar();
		return false;
	} else {
	    printf("file successfully loaded\n");
	}

    std::vector<GLfloat> texcoords;
    std::vector<GLuint> texcoords_indices;
    while( 1 ) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
            break; // EOF = End Of File. Quit the loop.
        // else : parse lineHeader
		if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			vertices.push_back((GLfloat)vertex.x/2);
			vertices.push_back((GLfloat)vertex.y/2);
			vertices.push_back((GLfloat)vertex.z/2);
		} else if (strcmp( lineHeader, "vt" ) == 0) {
		    // add texture coordinates in order
            glm::vec3 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            texcoords.push_back((GLfloat)uv.x);
            texcoords.push_back((GLfloat)uv.y);
		} else if ( strcmp( lineHeader, "f" ) == 0 ){
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3];
            int matches = fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
            if (matches != 6){
                printf("f format incompatible with current parser. Is the format in d/d d/d d/d?");
                fclose(file);
                return false;
            }
            indices.push_back((GLuint)vertexIndex[0] - 1);
            indices.push_back((GLuint)vertexIndex[1] - 1);
            indices.push_back((GLuint)vertexIndex[2] - 1);
            texcoords_indices.push_back(uvIndex[0] - 1);
            texcoords_indices.push_back(uvIndex[1] - 1);
            texcoords_indices.push_back(uvIndex[2] - 1);
        }
	}

    std::vector<GLfloat> interm_uvIndex(indices.size());

    // align the texture coordinate indices to be in the same order as vertex indices
    for( unsigned int i=0; i<indices.size(); i++ ){
        interm_uvIndex[indices[i]] = texcoords_indices[i];
    }
    // push back the texture coordinates to an output vector using the reordered indices
    for( unsigned int i=0; i<indices.size(); i++ ){
        out_texcoord.push_back(texcoords[2 * interm_uvIndex[i]]);
        out_texcoord.push_back(texcoords[2 * interm_uvIndex[i] + 1]);
    }
	fclose(file);
	return true;
}