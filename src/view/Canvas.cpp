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

	window.create(sf::VideoMode(500, font_size * (2 + 20 * track_height_ratio)),
				  "Mixeur musique",
				  sf::Style::Default,
				  settings);

	size_x = float(window.getSize().x);
	size_y = float(window.getSize().y);
}

/*
void Canvas::setup_view()
{
	float box_size = 1.0f;
	float x_min = 0;
	float x_max = 1 * box_size;
	float y_min = 0;
	float y_max = 5 * box_size;

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
}*/

void Canvas::start()
{
	open();
	//setup_view();
	while (window.isOpen())
	{
		handle_events();
		display_tracks();
		window.display();
	}
}

void Canvas::handle_events()
{
	if (state == PLAY &&
		music.getStatus() == sf::SoundSource::Status::Stopped)
	{
		if (playlist.loops[music_index])
		{ load_music(); }
		else if (music_index < playlist.size() - 1)
		{ next_track(); }
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
				//setup_view();
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

		case sf::Keyboard::Up:
			if (highlight_index == 0)
			{ break; }

			highlight_index--;
			break;

		case sf::Keyboard::Down:
			if (highlight_index == playlist.size() - 1)
			{ break; }

			highlight_index++;
			break;

		case sf::Keyboard::L:
			flip_highlighted_loop_boolean();
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

	//print_current_track();

	if (state == PLAY)
	{ music.play(); }
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

void Canvas::flip_highlighted_loop_boolean()
{
	playlist.loops[highlight_index] = !playlist.loops[highlight_index];
}

void Canvas::display_tracks()
{
	display_background();
	for (unsigned i = 0; i < playlist.size(); ++i)
	{ display_track(i); }

	if (state == PLAY)
	{ display_play_item(); }
	else
	{ display_pause_item(); }
}

void Canvas::display_background()
{
	sf::VertexArray quad(sf::Quads, 4);

	float xm = 0;
	float xM = 3000;
	float ym = 0;
	float yM = 2000;

	quad[0].position = sf::Vector2f(xm, ym);
	quad[1].position = sf::Vector2f(xm, yM);
	quad[2].position = sf::Vector2f(xM, yM);
	quad[3].position = sf::Vector2f(xM, ym);

	for (unsigned i = 0; i < 4; ++i)
	{ quad[i].color = sf::Color::White; }

	window.draw(quad);
}

void Canvas::display_track(unsigned i)
{
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(font_size);
	text.setString(playlist[i] + (playlist.loops[i] ? "   L" : ""));

	if (i == highlight_index)
	{ text.setStyle(sf::Text::Bold); }

	text.move(float(font_size) * (1 + track_height_ratio),
			  float(font_size) *
			  (1 + float(i) * track_height_ratio));
	text.setFillColor(sf::Color::Black);

	window.draw(text);
}

void Canvas::display_play_item()
{
	sf::VertexArray triangle(sf::Quads, 4);

	float x = float(font_size);
	float y = float(font_size) *
			  (1.1 + float(music_index) * track_height_ratio);
	float y1 = y + float(font_size);
	float x2 = x + float(font_size);
	float y2 = y + float(font_size) / 2;

	triangle[0].position = sf::Vector2f(x, y);
	triangle[1].position = sf::Vector2f(x, y1);
	triangle[2].position = sf::Vector2f(x2, y2);
	triangle[3].position = sf::Vector2f(x, y);

	for (unsigned i = 0; i < 4; ++i)
	{ triangle[i].color = sf::Color::Black; }

	window.draw(triangle);
}

void Canvas::display_pause_item()
{
	sf::VertexArray quad1(sf::Quads, 4);
	sf::VertexArray quad2(sf::Quads, 4);

	float xm = float(font_size);
	float xM = 2 * float(font_size);
	float ym = float(font_size) *
			   (1.1 + float(music_index) * track_height_ratio);
	float yM = ym + float(font_size);
	float quad_length_ratio = 0.28;

	quad1[0].position = sf::Vector2f(xm, ym);
	quad1[1].position = sf::Vector2f(xm, yM);
	quad1[2].position = sf::Vector2f(xm + quad_length_ratio *
										  float(font_size), yM);
	quad1[3].position = sf::Vector2f(xm + quad_length_ratio *
										  float(font_size), ym);

	quad2[0].position = sf::Vector2f(xm + (1 - quad_length_ratio) *
										  float(font_size), ym);
	quad2[1].position = sf::Vector2f(xm + (1 - quad_length_ratio) *
										  float(font_size), yM);
	quad2[2].position = sf::Vector2f(xM, yM);
	quad2[3].position = sf::Vector2f(xM, ym);

	for (unsigned i = 0; i < 4; ++i)
	{
		quad1[i].color = sf::Color::Black;
		quad2[i].color = sf::Color::Black;
	}

	window.draw(quad1);
	window.draw(quad2);
}













