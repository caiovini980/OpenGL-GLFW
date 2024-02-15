#include "DebugMethods.h"

void DebugMethods::CreateTriangleOnScreen()
{
	glBegin(GL_TRIANGLES);

	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0.0f, 0.5f);
	glVertex2f(0.5f, -0.5f);

	glEnd();
}
