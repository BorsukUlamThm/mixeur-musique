#include "Playlist.h"
#include <fstream>


Playlist::Playlist(const std::string& file)
{
	std::string path("../data/playlists/");
	path += file;
	std::ifstream ifs(path);

	std::string line;
	while (!ifs.eof())
	{
		std::getline(ifs, line);
		if (line[0] == '#')
		{ continue; }
		if (line.empty())
		{ continue; }

		add_track(line);
	}
}

std::string Playlist::operator[](unsigned int i) const
{
	return tracks[i];
}

std::string& Playlist::operator[](unsigned int i)
{
	return tracks[i];
}

bool Playlist::empty() const
{
	return tracks.empty();
}

unsigned Playlist::size() const
{
	return tracks.size();
}

void Playlist::add_track(const std::string& track)
{
	tracks.emplace_back(track);
}
