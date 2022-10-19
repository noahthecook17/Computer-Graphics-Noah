#ifndef OBB_H
#define OBB_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vector"
#include "mesh.h"
#include <glm/gtx/projection.hpp>

#include <glm/gtx/string_cast.hpp>


class OBB
{
private:

	glm::vec4 forward;
	glm::vec4 up;
	glm::vec4 right;

	float fdist;
	float udist;
	float rdist;

	vector<glm::vec4> vertices;

	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;

	glm::vec4 center;

	glm::vec3 defaultPosition;
	glm::vec3 defaultRotation;
	glm::vec4 defaultCenter;

	void updateVertices() {
		vertices.clear();
		vertices.push_back(center + (forward * fdist) + (up * udist) + (right * rdist));
		vertices.push_back(center + forward * fdist + up * udist + right * -rdist);
		vertices.push_back(center + forward * fdist + up * -udist + right * rdist);
		vertices.push_back(center + forward * fdist + up * -udist + right * -rdist);
		vertices.push_back(center + forward * -fdist + up * udist + right * rdist);
		vertices.push_back(center + forward * -fdist + up * udist + right * -rdist);
		vertices.push_back(center + forward * -fdist + up * -udist + right * rdist);
		vertices.push_back(center + forward * -fdist + up * -udist + right * -rdist);
	}

public:

	OBB(std::vector<Mesh> meshes) {

		bool firstLoop = true;

		translationMatrix = glm::mat4(1.0f);
		rotationMatrix = glm::mat4(1.0f);
		scaleMatrix = glm::mat4(1.0f);

		defaultRotation = glm::vec3(0.0f, 0.0f, 0.0f);
		defaultPosition = glm::vec3(0.0f, 0.0f, 0.0f);

		//cout << "translation matrix: " << glm::to_string(translationMatrix) << '\n';
		//cout << "rotation matrix: " << glm::to_string(rotationMatrix) << '\n';
		//cout << "scale matrix: " << glm::to_string(scaleMatrix) << '\n';

		forward = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		right = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

		float minX = 0.0f, maxX = 0.0f;
		float minY = 0.0f, maxY = 0.0f;
		float minZ = 0.0f, maxZ = 0.0f;

		for (int i = 0; i < meshes.size(); i++) {

			vector<Vertex> vertices = meshes[i].vertices;

			for (int i = 0; i < vertices.size(); i++) {

				glm::vec3 vertex = vertices[i].Position;

				if (firstLoop) {
					minX = vertex.x;
					maxX = vertex.x;
					minY = vertex.y;
					maxY = vertex.y;
					minZ = vertex.z;
					maxZ = vertex.z;
					firstLoop = false;
				}
				else {
					if (vertex.x < minX) minX = vertex.x;
					if (vertex.x > maxX) maxX = vertex.x;
					if (vertex.y < minY) minY = vertex.y;
					if (vertex.y > maxY) maxY = vertex.y;
					if (vertex.z < minZ) minZ = vertex.z;
					if (vertex.z > maxZ) maxZ = vertex.z;
				}
			}
		}
		//calculate the center
		fdist = glm::abs((maxX - minX) / 2);
		udist = glm::abs((maxY - minY) / 2);
		rdist = glm::abs((maxZ - minZ) / 2);

		center.x = minX + fdist;
		center.y = minY + udist;
		center.z = minZ + rdist;

		center.w = 1.0f;

		defaultCenter = center;

		updateVertices();
	}

	glm::vec3 getMovementFromOrigin() {
		return center - defaultCenter;
	}

	void setDefaults(glm::vec3 defaultPosition, glm::vec3 defaultRotation) {
		this->defaultPosition = defaultPosition;
		this->defaultRotation = defaultRotation;
	}

	void reset() {

		forward = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		right = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

		center = defaultCenter;

		translationMatrix = glm::inverse(translationMatrix);
		rotationMatrix = glm::inverse(rotationMatrix);

		updateVertices();

		translationMatrix = glm::mat4(1.0f);
		rotationMatrix = glm::mat4(1.0f);

		translate(defaultPosition);

		rotate(defaultRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		rotate(defaultRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		rotate(defaultRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));

	}

	glm::vec3 getUp() {
		return up;
	}

	glm::vec3 getForward() {
		return forward;
	}

	glm::vec3 getRight() {
		return right;
	}


	glm::vec3 getCenter() {
		return center;
	}

	OBB(glm::vec4 center, glm::vec4 forward, glm::vec4 up, glm::vec4 right, float fdist, float udist, float rdist) {
		this->center = center;
		this->forward = forward;
		this->up = up;
		this->right = right;
		this->fdist = fdist;
		this->udist = udist;
		this->rdist = rdist;
		this->rotationMatrix = glm::mat4(1.0f);
		this->translationMatrix = glm::mat4(1.0f);
		this->scaleMatrix = glm::mat4(1.0f);
	}

	

	OBB* getCopy() {
		return new OBB(this->center, this->forward, this->up, this->right, this->fdist, this->udist, this->rdist);
	}	

	void rotate(float angle, glm::vec3 axis) {	
		//translate to the origin and then do the rotation
		//glm::mat4 rotate = glm::translate(glm::mat4(1.0f), glm::vec3(center)) * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(axis)) * glm::translate(glm::mat4(1.0f), glm::vec3(-center));
		
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(axis));

		forward = rotate * forward;
		right = rotate * right;
		up = rotate * up;

		rotationMatrix = rotate * rotationMatrix;
		updateVertices();
	}

	void translate(glm::vec3 vector) {
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), vector);
		
		glm::vec4 tempCenter = translation * center;		
		
		center = tempCenter;
		translationMatrix = translation * translationMatrix;
		updateVertices();
	}


	void scale(float factor) {
		//glm::mat4 scale = glm::translate(glm::mat4(1.0f), glm::vec3(center)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f) * factor) * glm::translate(glm::mat4(1.0f), glm::vec3(-center));
		
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3((forward + up + right)) * factor);

		fdist *= factor;
		udist *= factor;
		rdist *= factor;

		scaleMatrix = scale * scaleMatrix;
		updateVertices();
	}

	void scaleForward(float factor) {
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(forward) * factor);

		fdist *= factor;

		scaleMatrix = scale * scaleMatrix;
		updateVertices();
	}

	glm::mat4 getModelTransform() {

		glm::mat4 scale = glm::translate(glm::mat4(1.0f), glm::vec3(center)) * scaleMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(-center));
		
		return (scale * translationMatrix * rotationMatrix );
	}


	bool collidesWith(OBB bb2) {

		bool debug = false;
		//glm::mat4 newBasis = glm::inverse(this->translationMatrix) * glm::inverse(this->rotationMatrix);
		glm::mat4 newBasis = glm::mat4(1.0f);

		vector<glm::vec3> convertedVertices = {
			glm::vec3(newBasis * bb2.vertices[0]),
			glm::vec3(newBasis * bb2.vertices[1]),
			glm::vec3(newBasis * bb2.vertices[2]),
			glm::vec3(newBasis * bb2.vertices[3]),
			glm::vec3(newBasis * bb2.vertices[4]),
			glm::vec3(newBasis * bb2.vertices[5]),
			glm::vec3(newBasis * bb2.vertices[6]),
			glm::vec3(newBasis * bb2.vertices[7]),
		};

		glm::vec3 cFront = newBasis * bb2.forward;
		glm::vec3 cUp = newBasis * bb2.up;
		glm::vec3 cRight = newBasis * bb2.right;
		
		if (debug) {
			cout << "param center: " << glm::to_string(bb2.center) << '\n';
			cout << "this center: " << glm::to_string(this->center) << '\n';
		}

		if (!overlapOnAxis(this->forward, convertedVertices)) {
			if(debug)
				cout << "NO OVERLAP 1" << '\n';
			return false;
		}
		if (!overlapOnAxis(this->up, convertedVertices)) {
			if (debug)
				cout << "NO OVERLAP 2" << '\n';
			return false;
		}
		if (!overlapOnAxis(this->right, convertedVertices)) {
			if (debug)
				cout << "NO OVERLAP 3" << '\n';
			return false;
		}
		if (!overlapOnAxis(cFront, convertedVertices)) {
			if (debug)
				cout << "NO OVERLAP 4" << '\n';
			return false;
		}
		if (!overlapOnAxis(cUp, convertedVertices)) {
			if (debug)
				cout << "NO OVERLAP 5" << '\n';
			return false;
		}
		if (!overlapOnAxis(cRight, convertedVertices)) {
			if (debug)
				cout << "NO OVERLAP 6" << '\n';
			return false;
		}
		if (!overlapOnAxis(glm::cross(glm::vec3(this->forward), cFront), convertedVertices)) {
			if (debug)
				cout << "NO OVERLAP 7" << '\n';
			return false;
		}
		if (!overlapOnAxis(glm::cross(glm::vec3(this->forward), cUp), convertedVertices)) {
			if (debug)
				cout << "NO OVERLAP 8" << '\n';
			return false;
		}
		if (!overlapOnAxis(glm::cross(glm::vec3(this->forward), cRight), convertedVertices)) {
			if (debug)
				cout << "NO OVERLAP 9" << '\n';
			return false;
		}

		if (!overlapOnAxis(glm::cross(glm::vec3(this->up), cFront), convertedVertices)) {
			if (debug)
				cout << "NO OVERLAP 10" << '\n';
			return false;
		}
		if (!overlapOnAxis(glm::cross(glm::vec3(this->up), cUp), convertedVertices)) {
			if (debug)
				cout << "NO OVERLAP 11" << '\n';
			return false;
		}
		if (!overlapOnAxis(glm::cross(glm::vec3(this->up), cRight), convertedVertices)) {
			if (debug)
				cout << "NO OVERLAP 12" << '\n';
			return false;
		}

		if (!overlapOnAxis(glm::cross(glm::vec3(this->right), cFront), convertedVertices)) {
			if (debug)
				cout << "NO OVERLAP 13" << '\n';
			return false;
		}
		if (!overlapOnAxis(glm::cross(glm::vec3(this->right), cUp), convertedVertices)) {
			if (debug)
				cout << "NO OVERLAP 14" << '\n';
			return false;
		}
		if (!overlapOnAxis(glm::cross(glm::vec3(this->right), cRight), convertedVertices)) {
			if (debug)
				cout << "NO OVERLAP 15" << '\n';
			return false;
		}

		return true;
		
	}

	bool overlapOnAxis(glm::vec3 axis, vector<glm::vec3> testv) {

		bool debug = false;

		if (debug) {
			cout << "-------------------------------" << '\n';
			cout << "axis: " << glm::to_string(axis) << '\n';
		}

		if (axis == glm::vec3(0.0f)) return true;

		bool first1 = true, first2 = true;
		float min1 = 0.0f, max1 = 0.0f;
		float min2 = 0.0f, max2 = 0.0f;
		float temp;
		for (int i = 0; i < testv.size(); i++) {

			if(debug)
				cout << "testv " << i << glm::to_string(testv[i]) << '\n';

			temp = glm::dot(testv[i], glm::normalize(axis));

			if (first1) {
				
				min1 = temp;
				max1 = temp;
				first1 = false;
			}
			else {
				if (temp < min1) min1 = temp;
				if (temp > max1) max1 = temp;
			}

		}
		for (int i = 0; i < vertices.size(); i++) {

			temp = glm::dot(glm::vec3(vertices[i]), glm::normalize(axis));

			if (first2) {

				min2 = temp;
				max2 = temp;
				first2 = false;
			}
			else {
				if (temp < min2) min2 = temp;
				if (temp > max2) max2 = temp;
			}

		}

		if (debug) {
			cout << "param min: " << min1 << '\n';
			cout << "param max: " << max1 << '\n' << '\n';	

			cout << "roatated min: " << min2 << '\n';
			cout << "roatated max: " << max2 << '\n' << '\n' << '\n';
		}
		

		if (min1 >= min2 && min1 <= max2) return true;

		else if (max1 < max2 && max1 > min2) return true;

		else if (min1 < min2 && max1 > max2) return true;

		else if (min2 < min1 && max2 > max2) return true;

		else return false;
	}

};
#endif

