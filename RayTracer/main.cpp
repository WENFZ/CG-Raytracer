

#include <iostream>
#include <algorithm>
#include <assert.h>
#include "scene_parser.h"
#include "Camera.h"
#include "Group.h"
#include "material.h"
#include "bitmap.h"
#include "rayTree.h"
#include "RayTracer.h"
#include "glCanvas.h"






SceneParser* scene = NULL;
RayTracer* rt = NULL;


char* input_file = NULL;
int width = 100;
int height = 100;
char* output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char* depth_file = NULL;
char* normal_file = NULL;
bool shade_back = false;
bool shadows = false;
int bounces = 5;
float weight = 0.01;
bool gui = false;

void parseArgs(int argc, char* argv[]);
void generatePhoto(char* filename,Vec3f(*generatePixel)(float, float));

Vec3f generateColorPiexel(float x, float y);
Vec3f generateDepthPiexel(float x, float y);
Vec3f generateNormalPiexel(float x, float y);
void traceRay(float x, float y);
void renderScene();

int main(int argc,char*argv[])
{

	parseArgs(argc, argv);
	if (input_file == NULL)
	{
		cout << "the input file should not be null\n";
		return 0;
	}
	SceneParser s(input_file);
	RayTracer rayTracer(&s, bounces, weight,shadows,shade_back);

	scene = &s;
	rt = &rayTracer;


	if (gui)
	{
		GLCanvas canvas;
		canvas.initialize(&s, renderScene, traceRay);
	}
	renderScene();


	cout << "程序结束\n";

	return 0;

}


void renderScene()
{
	if (output_file != NULL)
		generatePhoto(output_file,generateColorPiexel);
	if (depth_file != NULL)
		generatePhoto(depth_file, generateDepthPiexel);
	if(normal_file!=NULL)
		generatePhoto(normal_file, generateNormalPiexel);

}


Vec3f generateNormalPiexel(float x, float y)
{
	auto ray = scene->getCamera()->generateRay(Vec2f(x, y));
	Hit h(9999.0f, nullptr,Vec3f());
	scene->getGroup()->intersect(ray, h, depth_min);
	return Vec3f(abs(h.getNormal().x()), abs(h.getNormal().y()), abs(h.getNormal().z()));
}

Vec3f generateColorPiexel(float x, float y)
{
	Hit h(FLT_MAX, nullptr, Vec3f());
	auto ray = scene->getCamera()->generateRay(Vec2f(x, y));

	return rt->traceRay(ray, scene->getCamera()->getTMin(), 0, 1, 1, h);
}

Vec3f generateDepthPiexel(float x, float y)
{
	auto ray = scene->getCamera()->generateRay(Vec2f(x, y));
	Hit h(9999.0f, nullptr, Vec3f());
	scene->getGroup()->intersect(ray, h, depth_min);
	if (h.getT() < depth_max)
		return Vec3f(1, 1, 1) * (1 - (h.getT() - depth_min) / (depth_max - depth_min));
	else
		return Vec3f();
}

void generatePhoto(char* filename, Vec3f(*generatePixel)(float, float))
{
	printf("\nfilename：%s,height：%d，width：%d\n",filename,height,width);

	unsigned char* data = new unsigned char[height * width * 3];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			auto color = generatePixel(j*1.0/width, i*1.0/height);
			data[(i * width + j) * 3] = min(color.x() * 254,254.0f);
			data[(i * width + j) * 3 + 1] = min(color.y() * 254,254.0f);
			data[(i * width + j) * 3 + 2] = min(color.z() * 254, 254.0f);
		}
	}
	writeBMP(filename, width, height, data);
	delete[] data;
}

void parseArgs(int argc, char* argv[])
{
	// sample command line:
	// raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-input")) {
			i++; assert(i < argc);
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-size")) {
			i++; assert(i < argc);
			width = atoi(argv[i]);
			i++; assert(i < argc);
			height = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output")) {
			i++; assert(i < argc);
			output_file = argv[i];
		}
		else if (!strcmp(argv[i], "-depth")) {
			i++; assert(i < argc);
			depth_min = atof(argv[i]);
			i++; assert(i < argc);
			depth_max = atof(argv[i]);
			i++; assert(i < argc);
			depth_file = argv[i];
		}
		else if (!strcmp(argv[i], "-normals")) {
			i++; assert(i < argc);
			normal_file = argv[i];
		}
		else if (!strcmp(argv[i], "-shade_back")) {
			shade_back = true;
		}
		else if (!strcmp(argv[i], "-shadows")) {
			shadows = true;
		}
		else if (!strcmp(argv[i], "-bounces")) {
			i++; assert(i < argc);
			bounces = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-weight")) {
			i++; assert(i < argc);
			weight = atof(argv[i]);
		}
		else if (!strcmp(argv[i], "-gui")) {
			gui = true;
		}
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}
}


void traceRay(float x, float y)
{
	if (rt != nullptr)
	{
		Hit h(FLT_MAX, nullptr, Vec3f());
		auto ray = scene->getCamera()->generateRay(Vec2f(x, y));
		rt->traceRay(ray, scene->getCamera()->getTMin(), 0, 1, 1, h);
		RayTree::SetMainSegment(ray, 0, h.getT());
	}
}