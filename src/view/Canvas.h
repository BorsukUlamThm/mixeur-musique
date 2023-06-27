#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../model/Playlist.h"


class Canvas
{
protected:
	sf::RenderWindow window;
	//sf::View view;
	Playlist playlist;

	float size_x = 0;
	float size_y = 0;

	sf::Font font;
	unsigned font_size = 16;
	float track_height_ratio = 1.5;

	enum State
	{
		PAUSE,
		PLAY
	};
	State state = PAUSE;

	sf::Music music;
	unsigned music_index = 0;

public:
	Canvas() = default;
	explicit Canvas(Playlist playlist);
	void start();

protected:
	void setup();
	void open();
	//void setup_view();

	void handle_events();
	void handle_key_pressed_event(const sf::Event& event);

	void load_music();
	void print_current_track() const;
	void hard_reset();

	void prev_track();
	void next_track();

	void display_tracks();
	void display_background();
	void display_track(unsigned i);
	void display_play_item();
	void display_pause_item();
};

