export module window;

import <string>;

import <SFML/Graphics.hpp>;

import global_settings;


std::string MakeHeaderForWindow() 
{
	std::string app_header = "Asteroids! Patch: " + APP_VERSION;
	return app_header;
}


export void windowSettings( sf::RenderWindow& app_render_window ) 
{
	app_render_window.create( sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), MakeHeaderForWindow() );

	app_render_window.setFramerateLimit( FRAMERATE_LIMIT );

	app_render_window.setVerticalSyncEnabled( true );	// Limits framerate to display refresh rate
	app_render_window.setKeyRepeatEnabled( false );		// Disables key spamming event
}

