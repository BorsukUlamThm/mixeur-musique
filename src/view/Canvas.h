#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include "../model/Playlist.h"


struct Sound_player
{
	std::string name;
	sf::SoundBuffer buffer;
	sf::Sound player;

	Sound_player() = default;
};

class Canvas
{
protected:
	sf::RenderWindow window;
	float window_width = 500;
	Playlist playlist;

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
	unsigned highlight_index = 0;

	std::map<sf::Keyboard::Key, Sound_player> sounds;

public:
	Canvas() = default;
	Canvas(Playlist playlist, const std::string& sound_box_file);
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
	void flip_highlighted_loop_boolean();

	void display_all();
	void display_background();
	void display_tracks();
	void display_track(unsigned i);
	void display_play_item();
	void display_pause_item();
	void display_advancement_bar();
	void display_sounds();
};

