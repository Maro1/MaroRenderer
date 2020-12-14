#include "ObjectPicker.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstdio>
#include <string>

ObjectPicker::ObjectPicker(Scene* scene)
{
	m_Scene = scene;
	glGenTextures(1, &m_ColorTexture);
	glBindTexture(GL_TEXTURE_2D, m_ColorTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Scene->GetCamera()->GetViewPortSize().x, m_Scene->GetCamera()->GetViewPortSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

SceneNode* ObjectPicker::PickObject(MouseReleasedEvent* event)
{
	if (m_Scene->GetActors().empty()) {
		return nullptr;
	}
	if (event->GetX() < m_Scene->GetCamera()->GetViewPortPos().x || event->GetY() < m_Scene->GetCamera()->GetViewPortPos().y) {
		return nullptr;
	}
	m_Framebuffer.Bind();
	m_Framebuffer.AttachTexture(m_ColorTexture, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);
	RenderUniqueColors();
	GLint dimensions[4];
	glGetIntegerv(GL_VIEWPORT, dimensions);
	size_t width = dimensions[2];
	size_t height = dimensions[3];
	float rPixel;
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(event->GetX() - m_Scene->GetCamera()->GetViewPortPos().x, (height - event->GetY()) - m_Scene->GetCamera()->GetViewPortPos().y, 1, 1, GL_RED, GL_FLOAT, &rPixel);

	int pixel = rPixel * 255;

	if (m_ColorLookup.find(pixel) != m_ColorLookup.end())
	{
		return m_ColorLookup[pixel];
	}

	return nullptr;
}

void ObjectPicker::RenderImage()
{

	m_Framebuffer.Bind();
	FILE* output_image;
	int output_width, output_height;

	GLint dimensions[4];
	glGetIntegerv(GL_VIEWPORT, dimensions);

	output_width = dimensions[2];
	output_height = dimensions[3];

	/// READ THE PIXELS VALUES from FBO AND SAVE TO A .PPM FILE
	int             i, j, k;
	unsigned char* pixels = (unsigned char*)malloc(output_width * output_height * 3);

	/// READ THE CONTENT FROM THE FBO
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadPixels(0, 0, output_width, output_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	output_image = fopen("Assets/output.ppm", "wt");
	fprintf(output_image, "P3\n");
	fprintf(output_image, "# Created by Ricao\n");
	fprintf(output_image, "%d %d\n", output_width, output_height);
	fprintf(output_image, "255\n");

	k = 0;
	for (i = 0; i < output_width; i++)
	{
		for (j = 0; j < output_height; j++)
		{
			fprintf(output_image, "%u %u %u ", (unsigned int)pixels[k], (unsigned int)pixels[k + 1],
				(unsigned int)pixels[k + 2]);
			k = k + 3;
		}
		fprintf(output_image, "\n");
	}
	free(pixels);

	fclose(output_image);
}

void ObjectPicker::RenderUniqueColors()
{
	GLint dimensions[4];
	glGetIntegerv(GL_VIEWPORT, dimensions);
	GLint width = dimensions[2];
	GLint height = dimensions[3];
	glViewport(0, 0, m_Scene->GetCamera()->GetViewPortSize().x, m_Scene->GetCamera()->GetViewPortSize().y);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (SceneNode* node : m_Scene->GetActors())
	{
		int color = node->GetUUID();
		std::vector<Mesh*> meshes = node->GetModel()->GetMeshes();

		m_ColorLookup[color] = node;

		for (Mesh* mesh : meshes)
		{
			mesh->DrawColor(color, node->GetModelMatrix(), m_Scene->GetCamera()->GetView(), m_Scene->GetCamera()->GetProjection(), m_Scene->GetCamera()->GetPosition());
		}
	}
	glViewport(0, 0, width, height);
}
