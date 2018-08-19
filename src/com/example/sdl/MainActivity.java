package com.example.sdl;

import org.libsdl.app.SDLActivity;

import android.os.Bundle;

public class MainActivity extends SDLActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
//		setContentView(R.layout.activity_main);
		
//		test();
	}

	static native void test();
}
