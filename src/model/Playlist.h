#pragma once

#include <vector>
#include <string>


class Playlist
{
private:
	std::vector<std::string> tracks;

public:
	Playlist() = default;
	explicit Playlist(const std::string& file);

	std::string operator[](unsigned i) const;
	std::string& operator[](unsigned i);
	bool empty() const;
	unsigned size() const;

	void add_track(const std::string& track);
};

