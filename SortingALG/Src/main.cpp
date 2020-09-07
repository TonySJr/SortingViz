#include <algorithm>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <random>
#include <Windows.h>

#define Window_W 640
#define Window_H 480
#define onesixteen 0.01666666666666667
#define CharacterSize 24
#define screen (Window_W * 2) - 4
//	functions
void swap(int* arr, int y1, int y2);
void swap(sf::VertexArray &arr, int y1, int y2);
//	sort algorythms
void Selection_sort(sf::VertexArray& arr);
void quick_sort(sf::VertexArray& arr, int start, int end);
int partition(sf::VertexArray& arr, int start, int end);

//	draw things
void define_color(sf::VertexArray& arr, int i);
void draw(sf::RenderWindow& window, sf::VertexArray& Lines);



//	values
int testarr[5] = { 10,3,4,5,6 };
std::chrono::steady_clock::time_point start, end;

std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_int_distribution<> distrib(0, Window_H);

sf::RenderWindow window(sf::VideoMode(Window_W, Window_H), "Sort");

int main(void)
{
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << "Font not loaded properly\n";
		return EXIT_FAILURE;
	}
	sf::Text framerate;
	framerate.setFont(font); // font is a sf::Font
	framerate.setCharacterSize(CharacterSize); // in pixels, not points!
	framerate.setFillColor(sf::Color::Red);
	framerate.setPosition(0.f, 480.f - CharacterSize);
	framerate.setString("Sorted");

	//static int screen = (Window_W * 2) - 4;
	sf::VertexArray Lines(sf::Lines,screen);
	
	//	shuffle array
	for (int i = 0, s = 1,k = 1; ((i < screen) && (s < screen)); i += 2,s+=2)
	{
		Lines[i].position = sf::Vector2f((float)k, (float)Window_H-1);
		Lines[s].position = sf::Vector2f((float)k, (float)distrib(gen));
		k++;
	}
	
	// sort
	Selection_sort(Lines);
	//	shuffle array
	for (int i = 0, s = 1, k = 1; ((i < screen) && (s < screen)); i += 2, s += 2)
	{
		Lines[i].position = sf::Vector2f((float)k, (float)Window_H - 1);
		Lines[s].position = sf::Vector2f((float)k, (float)distrib(gen));
		k++;
	}
	quick_sort(Lines, 1, screen);
	define_color(Lines, screen);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		Sleep(50);
		if (true) // time to draw >
		{
			window.clear();
			//--------------------

			window.draw(Lines);
			window.draw(framerate);


			//--------------------
			window.display();
		}
	}
	return EXIT_SUCCESS;
}

void swap(int *arr, int y1, int y2)
{
	int temp = arr[y1];
	arr[y1] = arr[y2];
	arr[y2] = temp;
}

void swap(sf::VertexArray& arr, int y1, int y2)
{
	float temp = arr[y1].position.y;
	arr[y1].position.y = arr[y2].position.y;
	arr[y2].position.y = temp;
}

void Selection_sort(sf::VertexArray& arr)
{
	float min;
	int index;

	for(int k = 1; k < arr.getVertexCount();k+=2)
	{
		min = arr[k].position.y;
		index = k;
		for (int i = k+2; i < arr.getVertexCount(); i += 2)
		{
			if (arr[i].position.y < min)
			{
				min = arr[i].position.y;
				index = i;
			}
		}
		swap(arr, k, index);
		define_color(arr, screen);
		draw(window, arr);
	}
}
void quick_sort(sf::VertexArray& arr, int start, int end)
{

	if (start < end)
	{
		int index = partition(arr,start,end);
		quick_sort(arr, start, index - 2);
		quick_sort(arr, index + 2 ,end);
	}

}
int partition(sf::VertexArray& arr, int start, int end)
{
	int pivot_index = start;
	int pivot_value = arr[end].position.y;
	for (int k = pivot_index; k < end; k += 2)
	{
		if (arr[k].position.y < pivot_value)
		{
			swap(arr, k, pivot_index);
			pivot_index += 2;

			//	draw
			define_color(arr, screen);
			draw(window, arr);
		}
	}
	swap(arr, pivot_index, end);
	return pivot_index;
}
void define_color(sf::VertexArray& arr,int h)
{
	int k;
	for (int i = 0; i < h; i += 2)
	{
		k = i + 1;
		arr[i].color = sf::Color(0, (arr[k].position.y) / 2, 255 - (arr[k].position.y) / 2);
		arr[k].color = sf::Color(0, (arr[k].position.y) / 2, 255 - (arr[k].position.y) / 2);
	}
}
void draw(sf::RenderWindow &window,sf::VertexArray &Lines)
{
	if (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		//--------------------
		window.draw(Lines);
		//--------------------
		window.display();
		Sleep(5);
	}
}