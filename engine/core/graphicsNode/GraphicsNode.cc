#pragma once
#include "config.h"
#include "GraphicsNode.h"
#include "core/stb/stb_image.h"

GraphicsNode::GraphicsNode() {
	radiansX = 0.0f;
	radiansY = 0.0f;
}

GraphicsNode::GraphicsNode(const GraphicsNode& gN) {
	this->meshR = gN.meshR;
	this->meshVector = gN.meshVector;
	this->position = gN.position;
	this->radiansX = gN.radiansX;
	this->radiansY = gN.radiansY;
	this->shaderVector = gN.shaderVector;
	this->shadR = gN.shadR;
	this->textR = gN.textR;
	this->textureVector = gN.textureVector;
	this->scaling = gN.scaling;
}

GraphicsNode::~GraphicsNode() {
	
}

 GraphicsNode::GraphicsNode(GraphicsNode&& move) noexcept{

}

GraphicsNode& GraphicsNode::operator=(GraphicsNode&& rhs) noexcept {
	return rhs;
}


void GraphicsNode::DrawQuads(mat4 viewProjection) {
		//Cycle through primitives
		for (int j = 0; j < this->meshR->primitiveVector.size(); j++) {
			primitive currentPrim = this->meshR->primitiveVector[j];
			this->meshR->BindBuffer(currentPrim.vertBuffer);
			this->textR->BindTexture(this->textR->texture);

			//Prepare model matrix
			mat4 modelMatrix = mat4(
				{ 1,0,0,0 },
				{ 0,1,0,0 },
				{ 0,0,1,0 },
				{ position.x,position.y,position.z,1 }
			);
			modelMatrix = modelMatrix * rotationx(this->radiansX) * rotationy(this->radiansY);
			mat4 mvp = viewProjection * modelMatrix;

			//Send mvp to shader
			this->shadR->UseProgram(this->shadR->shaderProgram);
			this->shadR->EditUniformMat4fv(this->shadR->shaderProgram, "mvp", mvp);
			//Send texture to shader
			this->shadR->EditUniform1i(this->shadR->shaderProgram, "tex");

			this->meshR->DrawQuads();

			this->meshR->BindBuffer(0);
			this->textR->BindTexture(0);
			this->shadR->UseProgram(0);

		}

}

void GraphicsNode::DrawTrisDeferred(mat4 view, mat4 projection, float scaling, std::shared_ptr<GLuint> geometryShaderProgram) {
	ShaderResource shadR;
	for (int i = 0; i < meshVector.size(); i++) { //For each mesh
		MeshResource* currentMesh = meshVector[i].get();
		for (int j = 0; j < currentMesh->primitiveVector.size(); j++) { //For each primitive
			primitive const currentPrim = currentMesh->primitiveVector[j];

			//Prepare model matrix
			mat4 modelMatrix = mat4(
				{ scaling,0,0,0 },
				{ 0,scaling,0,0 },
				{ 0,0,scaling,0 },
				{ position.x,position.y,position.z,1 }
			);

			modelMatrix = modelMatrix * rotationx(this->radiansX) * rotationy(this->radiansY);
			mat4 mvp = projection * view * modelMatrix;

			vec3 camPos = vec3(view[3][0], view[3][1], view[3][2]);
			shadR.UseProgram(geometryShaderProgram);

			std::shared_ptr<GLuint> shaderProgram = geometryShaderProgram;

			std::vector<GLuint> textures = currentPrim.textures;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures[0]);
			shadR.EditUniform1i(shaderProgram, "tex", 0);
			if (textures.size() > 1) {
				glActiveTexture(GL_TEXTURE0 + 1);
				glBindTexture(GL_TEXTURE_2D, textures[1]);
				shadR.EditUniform1i(shaderProgram, "normalMap", 1);
			}

			shadR.EditUniformMat4fv(shaderProgram, "mvp", mvp);
			shadR.EditUniformMat4fv(shaderProgram, "m", modelMatrix);
			shadR.EditUniform3fv(shaderProgram, "camPos", camPos);
			shadR.EditUniformMat4fv(shaderProgram, "mTransposedInverse", transpose(inverse(modelMatrix)));


			this->meshVector[i]->DrawTris(currentPrim);
			shadR.UseProgram(0);
			glActiveTexture(GL_TEXTURE0);
		}
	}
}

void GraphicsNode::DrawTris(mat4 view, mat4 projection, float scaling) {
	ShaderResource shadR;
	for (int i = 0; i < meshVector.size(); i++) { //For each mesh
		MeshResource* currentMesh = meshVector[i].get();
		for (int j = 0; j < currentMesh->primitiveVector.size(); j++) { //For each primitive
			primitive const currentPrim = currentMesh->primitiveVector[j];

			//Prepare model matrix
			mat4 modelMatrix = mat4(
				{ scaling,0,0,0 },
				{ 0,scaling,0,0 },
				{ 0,0,scaling,0 },
				{ position.x,position.y,position.z,1 }
			);
			
			modelMatrix = modelMatrix * rotationx(this->radiansX) * rotationy(this->radiansY);
			mat4 mvp = projection * view * modelMatrix;
			
			vec3 camPos = vec3(view[3][0], view[3][1], view[3][2]);

			if (currentPrim.textures.size() != 0)
				currentPrim.material->ApplyMaterial(currentPrim.textures);
			else
				currentPrim.material->ApplyMaterial();

			std::shared_ptr<GLuint> shaderProgram = currentPrim.material->shaderProgram;
			shadR.EditUniformMat4fv(shaderProgram, "mvp", mvp);
			shadR.EditUniformMat4fv(shaderProgram, "m", modelMatrix);
			shadR.EditUniform3fv(shaderProgram, "camPos", camPos);
			shadR.EditUniformMat4fv(shaderProgram, "mTransposedInverse", transpose(inverse(modelMatrix)));


			this->meshVector[i]->DrawTris(currentPrim);
			currentPrim.material->UnbindMaterial();
		}
			
	}
}

void GraphicsNode::LoadGLTF(std::string filename) {
	fx::gltf::Document doc = fx::gltf::LoadFromText(filename);
	
	//Used if no targets are specified in gltf file
	std::vector<GLuint> targetLocations;

	//Set up buffers
	std::vector<GLuint> buffers;
	buffers.reserve(doc.bufferViews.size());
	for (int i = 0; i < doc.bufferViews.size(); i++) {
		buffers.push_back(0);
	}

	//Buffer all data
	glGenBuffers((GLsizei)doc.bufferViews.size(), &buffers[0]);
	for (int i = 0; i < doc.bufferViews.size(); i++) {
		fx::gltf::BufferView bufferView = doc.bufferViews[i];
		
		GLenum target = (GLenum)bufferView.target;
		
		//Cycle through all primitives to see if current bufferview is used as index buffer anywhere
		if (target == 0) {
			bool indexBuffer = false;
			for (int j = 0; j < doc.meshes.size(); j++) {
				for (int k = 0; k < doc.meshes[j].primitives.size();k++) {
					if (doc.meshes[j].primitives[k].indices == i) {
						indexBuffer = true;
					}
				}
			}
			if (indexBuffer){
				target = GL_ELEMENT_ARRAY_BUFFER;
			}
			else {
				target = GL_ARRAY_BUFFER;
			}
			targetLocations.push_back(target);
		}


		glBindBuffer(target, buffers[i]);

		glBufferData(target, bufferView.byteLength, &(doc.buffers[bufferView.buffer].data[0]) + bufferView.byteOffset, GL_STATIC_DRAW);
	}


	for (int i = 0; i < doc.meshes.size(); i++) { //For each mesh
		std::shared_ptr<MeshResource> meshR = std::make_shared<MeshResource>();
		for (int j = 0; j < doc.meshes[i].primitives.size(); j++) { //For each primitive
			fx::gltf::Primitive currentPrim = doc.meshes[i].primitives[j];
			primitive prim;
			glGenVertexArrays(1, &prim.vertexArray);
			meshR->BindVertexArray(prim);

			//ATTTRIBUTES
			fx::gltf::Attributes attributes = currentPrim.attributes;
			for (int k = 0; k < attributes.size(); k++) {
				GLuint accessorLocation = attributes[this->getAccessorString(k)]; //Find correct type of value based on current index
				GLuint bufferLocation = doc.accessors[accessorLocation].bufferView;

				GLuint target;
				if (targetLocations.size() == 0) {
					target = GLint(doc.bufferViews[doc.accessors[accessorLocation].bufferView].target);
				}
				else {
					target = targetLocations[bufferLocation];
				}


				glBindBuffer(target, buffers[bufferLocation]);
				GLint type = (GLint)doc.accessors[accessorLocation].type;
				GLenum componentType = (GLenum)doc.accessors[accessorLocation].componentType;
				GLenum normalized = (GLenum)doc.accessors[accessorLocation].normalized;
				GLuint stride = doc.bufferViews[bufferLocation].byteStride;
				GLuint offset = doc.accessors[accessorLocation].byteOffset;

				glEnableVertexAttribArray(k);
				glVertexAttribPointer(k, type, componentType, normalized, stride, (void*)offset);
				glBindBuffer(target, 0);
			}


			//TEXTURES

			//If URI
			fx::gltf::Material primitiveMaterial = doc.materials[currentPrim.material];
			std::vector<std::string> pathStrings;
			std::string directoryname = filename.substr(0, filename.find_last_of('/') + 1);

			GLuint baseColourTextureIndex = primitiveMaterial.pbrMetallicRoughness.baseColorTexture.index;
			std::string baseColourTexturePath = directoryname + doc.images[baseColourTextureIndex].uri;
			pathStrings.push_back(baseColourTexturePath);

			GLuint normalTextureIndex = primitiveMaterial.normalTexture.index;
			if(normalTextureIndex != -1){
				std::string normalTexturePath = directoryname + doc.images[normalTextureIndex].uri;
				pathStrings.push_back(normalTexturePath);
			}


			for (int i = 0; i < pathStrings.size(); i++) {
				int width, height, channels;
				unsigned char* data = stbi_load(pathStrings[i].c_str(), &width, &height, &channels, 0);

				//Check if fileformat supports RGB/RGBA
				GLenum fileChannels = -1;
				if (channels == 3) {
					fileChannels = GL_RGB;
				}
				else if (channels == 4) {
					fileChannels = GL_RGBA;
				}

				prim.textures.push_back(0);

				glGenTextures(1, &prim.textures.back());
				glBindTexture(GL_TEXTURE_2D, prim.textures.back());
				if (data) {
					glTexImage2D(GL_TEXTURE_2D, 0, fileChannels, width, height, 0, fileChannels, GL_UNSIGNED_BYTE, data);

					glGenerateMipmap(GL_TEXTURE_2D);
				}
				else {
					std::cout << "No data found" << "\n";
				}

				glBindTexture(GL_TEXTURE_2D, 0); 
				stbi_image_free(data);
			}






			//Index buffer
			int indexLocation = doc.meshes[i].primitives[j].indices;
			prim.indexBuffer = buffers[doc.accessors[indexLocation].bufferView];

			meshR->BindIndexBuffer(prim);
			prim.numIndices = doc.accessors[indexLocation].count;
			prim.indexType = (GLenum)doc.accessors[indexLocation].componentType;
			prim.indexOffset = doc.accessors[indexLocation].byteOffset;


			meshR->UnbindVertexArray();
			meshR->UnbindIndexBuffer();//Unbind index buffer after unbinding VAO to ensure index buffer is stored in VAO
			meshR->primitiveVector.push_back(prim);
		}

		this->meshVector.push_back(meshR);
	}
}

std::string GraphicsNode::getAccessorString(int attribArrayIndex) {
	switch (attribArrayIndex) {
		case 0:	
			return "POSITION";
			break;
		case 1:
			return "TEXCOORD_0";
			break;
		case 2:
			return "NORMAL";
			break;
		case 3:
			return "TANGENT";
			break;
	}
	return "NONE";
}