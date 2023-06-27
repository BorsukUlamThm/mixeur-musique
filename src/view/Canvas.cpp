#include "Canvas.h"
#include <iostream>


void Canvas::setup()
{
	font.loadFromFile("../data/fonts/arial.ttf");
	load_music();
}

Canvas::Canvas(Playlist playlist) :
		playlist(std::move(playlist))
{
	setup();
}

void Canvas::open()
{
	window.clear(sf::Color::White);
	if (window.isOpen())
	{ return; }

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	window.create(sf::VideoMode::getDesktopMode(),
				  "Mixeur musique",
				  sf::Style::Default,
				  settings);

	size_x = float(window.getSize().x);
	size_y = float(window.getSize().y);
}

void Canvas::setup_view()
{
	float box_size = 1.0f;
	float x_min = -box_size;
	float x_max = box_size;
	float y_min = -box_size;
	float y_max = box_size;

	float window_format = size_x / size_y;
	float figure_format = (x_max - x_min) / (y_max - y_min);
	float x_length = (x_max - x_min);
	float y_length = (y_max - y_min);

	if (window_format < figure_format)
	{ view.setSize(x_length, x_length / window_format); }
	else
	{ view.setSize(y_length * window_format, y_length); }

	view.setCenter((x_min + x_max) / 2, (y_min + y_max) / 2);
	window.setView(view);
}

void Canvas::start()
{
	open();
	setup_view();
	while (window.isOpen())
	{
		handle_events();
		window.display();
	}
}

void Canvas::handle_events()
{
	if (state == PLAY &&
		music_index < playlist.size() - 1 &&
		music.getStatus() == sf::SoundSource::Status::Stopped)
	{
		next_track();
	}

	sf::Event event {};
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::Resized:
				size_x = float(event.size.width);
				size_y = float(event.size.height);
				setup_view();
				break;

			case sf::Event::KeyPressed:
				handle_key_pressed_event(event);
				break;

			default:
				break;
		}
	}
}

void Canvas::handle_key_pressed_event(const sf::Event& event)
{
	switch (event.key.code)
	{
		case sf::Keyboard::Escape:
			window.clear();
			window.close();
			break;

		case sf::Keyboard::Enter:
			hard_reset();
			break;

		case sf::Keyboard::Space:
			if (state == PAUSE)
			{
				music.play();
				state = PLAY;
			}
			else if (state == PLAY)
			{
				music.pause();
				state = PAUSE;
			}
			break;

		case sf::Keyboard::Left:
			if (music_index == 0)
			{ break; }

			prev_track();
			break;

		case sf::Keyboard::Right:
			if (music_index == playlist.size() - 1)
			{ break; }

			next_track();
			break;

		default:
			break;
	}
}

void Canvas::load_music()
{
	if (playlist.empty())
	{ return; }

	std::string musics_dir = "../data/musiques/";
	music.openFromFile(musics_dir + playlist[music_index] + ".wav");

	print_current_track();

	if (state == PLAY)
	{
		music.play();
	}
}

void Canvas::print_current_track() const
{
	std::cout << music_index + 1
			  << " : "
			  << playlist[music_index]
			  << std::endl;
}

void Canvas::hard_reset()
{
	music.stop();
	music_index = 0;
	setup();
	state = PAUSE;
}

void Canvas::prev_track()
{
	music_index--;
	setup();
}

void Canvas::next_track()
{
	music_index++;
	setup();
}
















