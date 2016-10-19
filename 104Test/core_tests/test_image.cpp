//
// Created by Vadim Fedorov on 18/10/16.
//

#include "gtest/gtest.h"
#include "image.h"

TEST(Image, ImageFx_ctors) {
	const float init_val = 1.0f;
	const int w = 5;
	const int h = 4;
	const int c = 3;
	const Shape sz(w, h);

	// ImageFx()
	{
		ImageFx<float> im;
		EXPECT_FALSE((bool)im);
		EXPECT_TRUE(im.is_empty());
		EXPECT_EQ(im.size_x(), 0);
		EXPECT_EQ(im.size_y(), 0);
		EXPECT_EQ(im.size(), Shape(0, 0));
		EXPECT_EQ(im.number_of_channels(), 0);
		EXPECT_EQ(im.raw_length(), 0);
		EXPECT_EQ(im.number_of_pixels(), 0);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		EXPECT_TRUE(!im.raw());
	}

	// ImageFx(uint size_x, uint size_y);
	{
		ImageFx<float> im(w, h);
		EXPECT_TRUE((bool)im);
		EXPECT_FALSE(im.is_empty());
		EXPECT_EQ(im.size_x(), w);
		EXPECT_EQ(im.size_y(), h);
		EXPECT_EQ(im.size(), Shape(w, h));
		EXPECT_EQ(im.number_of_channels(), 1);
		EXPECT_EQ(im.raw_length(), w * h * 1);
		EXPECT_EQ(im.number_of_pixels(), w * h);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], 0.0f);
		}
	}

	// ImageFx(uint size_x, uint size_y, uint number_of_channels);
	{
		ImageFx<float> im(w, h, (uint)c);
		EXPECT_TRUE((bool)im);
		EXPECT_FALSE(im.is_empty());
		EXPECT_EQ(im.size_x(), w);
		EXPECT_EQ(im.size_y(), h);
		EXPECT_EQ(im.size(), Shape(w, h));
		EXPECT_EQ(im.number_of_channels(), c);
		EXPECT_EQ(im.raw_length(), w * h * c);
		EXPECT_EQ(im.number_of_pixels(), w * h);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], 0.0f);
		}
	}

	// ImageFx(uint size_x, uint size_y, T default_value);
	{
		ImageFx<float> im(w, h, init_val);
		EXPECT_TRUE((bool)im);
		EXPECT_FALSE(im.is_empty());
		EXPECT_EQ(im.size_x(), w);
		EXPECT_EQ(im.size_y(), h);
		EXPECT_EQ(im.size(), Shape(w, h));
		EXPECT_EQ(im.number_of_channels(), 1);
		EXPECT_EQ(im.raw_length(), w * h * 1);
		EXPECT_EQ(im.number_of_pixels(), w * h);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], init_val);
		}
	}

	// ImageFx(uint size_x, uint size_y, uint number_of_channels, T default_value);
	{
		ImageFx<float> im(w, h, c, init_val);
		EXPECT_TRUE((bool)im);
		EXPECT_FALSE(im.is_empty());
		EXPECT_EQ(im.size_x(), w);
		EXPECT_EQ(im.size_y(), h);
		EXPECT_EQ(im.size(), Shape(w, h));
		EXPECT_EQ(im.number_of_channels(), c);
		EXPECT_EQ(im.raw_length(), w * h * c);
		EXPECT_EQ(im.number_of_pixels(), w * h);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], init_val);
		}
	}

	// ImageFx(Shape size);
	{
		ImageFx<float> im(sz);
		EXPECT_TRUE((bool)im);
		EXPECT_FALSE(im.is_empty());
		EXPECT_EQ(im.size_x(), w);
		EXPECT_EQ(im.size_y(), h);
		EXPECT_EQ(im.size(), Shape(w, h));
		EXPECT_EQ(im.number_of_channels(), 1);
		EXPECT_EQ(im.raw_length(), w * h * 1);
		EXPECT_EQ(im.number_of_pixels(), w * h);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], 0.0f);
		}
	}

	// ImageFx(Shape size, uint number_of_channels);
	{
		ImageFx<float> im(sz, (uint)c);
		EXPECT_TRUE((bool)im);
		EXPECT_FALSE(im.is_empty());
		EXPECT_EQ(im.size_x(), w);
		EXPECT_EQ(im.size_y(), h);
		EXPECT_EQ(im.size(), Shape(w, h));
		EXPECT_EQ(im.number_of_channels(), c);
		EXPECT_EQ(im.raw_length(), w * h * c);
		EXPECT_EQ(im.number_of_pixels(), w * h);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], 0.0f);
		}
	}

	// ImageFx(Shape size, T default_value);
	{
		ImageFx<float> im(sz, init_val);
		EXPECT_TRUE((bool)im);
		EXPECT_FALSE(im.is_empty());
		EXPECT_EQ(im.size_x(), w);
		EXPECT_EQ(im.size_y(), h);
		EXPECT_EQ(im.size(), Shape(w, h));
		EXPECT_EQ(im.number_of_channels(), 1);
		EXPECT_EQ(im.raw_length(), w * h * 1);
		EXPECT_EQ(im.number_of_pixels(), w * h);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], init_val);
		}
	}

	// ImageFx(Shape size, uint number_of_channels, T default_value);
	{
		ImageFx<float> im(sz, c, init_val);
		EXPECT_TRUE((bool)im);
		EXPECT_FALSE(im.is_empty());
		EXPECT_EQ(im.size_x(), w);
		EXPECT_EQ(im.size_y(), h);
		EXPECT_EQ(im.size(), Shape(w, h));
		EXPECT_EQ(im.number_of_channels(), c);
		EXPECT_EQ(im.raw_length(), w * h * c);
		EXPECT_EQ(im.number_of_pixels(), w * h);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], init_val);
		}
	}
}


TEST(Image, Image_ctors) {
	const float init_val = 1.0f;
	const int w = 5;
	const int h = 4;
	const int c = 3;
	const Shape sz(w, h);

	// Image()
	{
		Image<float> im;
		EXPECT_FALSE((bool)im);
		EXPECT_TRUE(im.is_empty());
		EXPECT_EQ(im.size_x(), 0);
		EXPECT_EQ(im.size_y(), 0);
		EXPECT_EQ(im.size(), Shape(0, 0));
		EXPECT_EQ(im.number_of_channels(), 0);
		EXPECT_EQ(im.raw_length(), 0);
		EXPECT_EQ(im.number_of_pixels(), 0);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		EXPECT_TRUE(!im.raw());
	}

	// Image(uint size_x, uint size_y);
	{
		Image<float> im(w, h);
		EXPECT_TRUE((bool)im);
		EXPECT_FALSE(im.is_empty());
		EXPECT_EQ(im.size_x(), w);
		EXPECT_EQ(im.size_y(), h);
		EXPECT_EQ(im.size(), Shape(w, h));
		EXPECT_EQ(im.number_of_channels(), 1);
		EXPECT_EQ(im.raw_length(), w * h * 1);
		EXPECT_EQ(im.number_of_pixels(), w * h);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], 0.0f);
		}
	}

	// Image(uint size_x, uint size_y, uint number_of_channels);
	{
		Image<float> im(w, h, (uint)c);
		EXPECT_TRUE((bool)im);
		EXPECT_FALSE(im.is_empty());
		EXPECT_EQ(im.size_x(), w);
		EXPECT_EQ(im.size_y(), h);
		EXPECT_EQ(im.size(), Shape(w, h));
		EXPECT_EQ(im.number_of_channels(), c);
		EXPECT_EQ(im.raw_length(), w * h * c);
		EXPECT_EQ(im.number_of_pixels(), w * h);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], 0.0f);
		}
	}

	// Image(uint size_x, uint size_y, T default_value);
	{
		Image<float> im(w, h, init_val);
		EXPECT_TRUE((bool)im);
		EXPECT_FALSE(im.is_empty());
		EXPECT_EQ(im.size_x(), w);
		EXPECT_EQ(im.size_y(), h);
		EXPECT_EQ(im.size(), Shape(w, h));
		EXPECT_EQ(im.number_of_channels(), 1);
		EXPECT_EQ(im.raw_length(), w * h * 1);
		EXPECT_EQ(im.number_of_pixels(), w * h);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], init_val);
		}
	}

	// Image(uint size_x, uint size_y, uint number_of_channels, T default_value);
	{
		Image<float> im(w, h, c, init_val);
		EXPECT_TRUE((bool)im);
		EXPECT_FALSE(im.is_empty());
		EXPECT_EQ(im.size_x(), w);
		EXPECT_EQ(im.size_y(), h);
		EXPECT_EQ(im.size(), Shape(w, h));
		EXPECT_EQ(im.number_of_channels(), c);
		EXPECT_EQ(im.raw_length(), w * h * c);
		EXPECT_EQ(im.number_of_pixels(), w * h);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], init_val);
		}
	}

	// Image(Shape size);
	{
		Image<float> im(sz);
		EXPECT_TRUE((bool)im);
		EXPECT_FALSE(im.is_empty());
		EXPECT_EQ(im.size_x(), w);
		EXPECT_EQ(im.size_y(), h);
		EXPECT_EQ(im.size(), Shape(w, h));
		EXPECT_EQ(im.number_of_channels(), 1);
		EXPECT_EQ(im.raw_length(), w * h * 1);
		EXPECT_EQ(im.number_of_pixels(), w * h);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], 0.0f);
		}
	}

	// Image(Shape size, uint number_of_channels);
	{
		Image<float> im(sz, (uint)c);
		EXPECT_TRUE((bool)im);
		EXPECT_FALSE(im.is_empty());
		EXPECT_EQ(im.size_x(), w);
		EXPECT_EQ(im.size_y(), h);
		EXPECT_EQ(im.size(), Shape(w, h));
		EXPECT_EQ(im.number_of_channels(), c);
		EXPECT_EQ(im.raw_length(), w * h * c);
		EXPECT_EQ(im.number_of_pixels(), w * h);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], 0.0f);
		}
	}

	// Image(Shape size, T default_value);
	{
		Image<float> im(sz, init_val);
		EXPECT_TRUE((bool)im);
		EXPECT_FALSE(im.is_empty());
		EXPECT_EQ(im.size_x(), w);
		EXPECT_EQ(im.size_y(), h);
		EXPECT_EQ(im.size(), Shape(w, h));
		EXPECT_EQ(im.number_of_channels(), 1);
		EXPECT_EQ(im.raw_length(), w * h * 1);
		EXPECT_EQ(im.number_of_pixels(), w * h);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], init_val);
		}
	}

	// Image(Shape size, uint number_of_channels, T default_value);
	{
		Image<float> im(sz, c, init_val);
		EXPECT_TRUE((bool)im);
		EXPECT_FALSE(im.is_empty());
		EXPECT_EQ(im.size_x(), w);
		EXPECT_EQ(im.size_y(), h);
		EXPECT_EQ(im.size(), Shape(w, h));
		EXPECT_EQ(im.number_of_channels(), c);
		EXPECT_EQ(im.raw_length(), w * h * c);
		EXPECT_EQ(im.number_of_pixels(), w * h);
		EXPECT_EQ(im.color_space(), ColorSpaces::unknown);
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], init_val);
		}
	}
}


// Image -> ImageFx : data sharing
TEST(Image, Image_to_ImageFx) {

	// First test copy ctor
	{
		const float init_val = 1.0f;
		Image<float> im(2, 2, 1, init_val);
		ImageFx<float> imfx(im);

		// Check that initial values were copied correctly
		const float *datafx = imfx.raw();
		for (int i = 0; i < imfx.raw_length(); ++i) {
			EXPECT_EQ(datafx[i], init_val);
		}

		// Check that both im and imfx share the data
		EXPECT_EQ(im.raw(), imfx.raw());
		const float new_val = 2.0f;
		const Point p(0, 1);
		im(p) = new_val;
		EXPECT_EQ(imfx(p), new_val);

		// Check that the metadata is the same
		EXPECT_EQ(im.size(), imfx.size());
		EXPECT_EQ(im.number_of_channels(), imfx.number_of_channels());
		EXPECT_EQ(im.color_space(), imfx.color_space());
	}

	// Then test assignment operator
	{
		const float init_val = 3.0f;
		Image<float> im(2, 2, 1, init_val);
		ImageFx<float> imfx;
		imfx = im;

		// Check that initial values were copied correctly
		const float *datafx = imfx.raw();
		for (int i = 0; i < imfx.raw_length(); ++i) {
			EXPECT_EQ(datafx[i], init_val);
		}

		// Check that both im and imfx share the data
		EXPECT_EQ(im.raw(), imfx.raw());
		const float new_val = 4.0f;
		const Point p(1, 0);
		im(p) = new_val;
		EXPECT_EQ(imfx(p), new_val);

		// Check that the metadata is the same
		EXPECT_EQ(im.size(), imfx.size());
		EXPECT_EQ(im.number_of_channels(), imfx.number_of_channels());
		EXPECT_EQ(im.color_space(), imfx.color_space());
	}
}


// Image -> Image : data sharing
TEST(Image, Image_to_Image) {

	// First test copy ctor
	{
		const float init_val = 1.0f;
		Image<float> im1(2, 2, 1, init_val);
		Image<float> im2(im1);

		// Check that initial values were copied correctly
		const float *data2 = im2.raw();
		for (int i = 0; i < im2.raw_length(); ++i) {
			EXPECT_EQ(data2[i], init_val);
		}

		// Check that both im1 and im2 share the data
		EXPECT_EQ(im1.raw(), im2.raw());
		const float new_val = 2.0f;
		const Point p(1, 0);
		im1(p) = new_val;
		EXPECT_EQ(im2(p), new_val);

		// Check that the metadata is the same
		EXPECT_EQ(im1.size(), im2.size());
		EXPECT_EQ(im1.number_of_channels(), im2.number_of_channels());
		EXPECT_EQ(im1.color_space(), im2.color_space());
	}

	// Then test assignment operator
	{
		const float init_val = 1.0f;
		Image<float> im1(2, 2, 1, init_val);
		Image<float> im2;
		im2 = im1;

		// Check that initial values were copied correctly
		const float *data2 = im2.raw();
		for (int i = 0; i < im2.raw_length(); ++i) {
			EXPECT_EQ(data2[i], init_val);
		}

		// Check that both im1 and im2 share the data
		EXPECT_EQ(im1.raw(), im2.raw());
		const float new_val = 2.0f;
		const Point p(1, 1);
		im1(p) = new_val;
		EXPECT_EQ(im2(p), new_val);

		// Check that the metadata is the same
		EXPECT_EQ(im1.size(), im2.size());
		EXPECT_EQ(im1.number_of_channels(), im2.number_of_channels());
		EXPECT_EQ(im1.color_space(), im2.color_space());
	}
}


// ImageFx -> ImageFx : data sharing
TEST(Image, ImageFx_to_ImageFx) {

	// First test copy ctor
	{
		const float init_val = 1.0f;
		ImageFx<float> im1(2, 2, 1, init_val);
		ImageFx<float> im2(im1);

		// Check that initial values were copied correctly
		const float *data2 = im2.raw();
		for (int i = 0; i < im2.raw_length(); ++i) {
			EXPECT_EQ(data2[i], init_val);
		}

		// Check that both im1 and im2 share the data
		EXPECT_EQ(im1.raw(), im2.raw());

		// Check that the metadata is the same
		EXPECT_EQ(im1.size(), im2.size());
		EXPECT_EQ(im1.number_of_channels(), im2.number_of_channels());
		EXPECT_EQ(im1.color_space(), im2.color_space());
	}

	// Then test assignment operator
	{
		const float init_val = 1.0f;
		ImageFx<float> im1(2, 2, 1, init_val);
		ImageFx<float> im2;
		im2 = im1;

		// Check that initial values were copied correctly
		const float *data2 = im2.raw();
		for (int i = 0; i < im2.raw_length(); ++i) {
			EXPECT_EQ(data2[i], init_val);
		}

		// Check that both im1 and im2 share the data
		EXPECT_EQ(im1.raw(), im2.raw());

		// Check that the metadata is the same
		EXPECT_EQ(im1.size(), im2.size());
		EXPECT_EQ(im1.number_of_channels(), im2.number_of_channels());
		EXPECT_EQ(im1.color_space(), im2.color_space());
	}
}


// ImageFx -> Image : data copying
TEST(Image, ImageFx_to_Image) {

	// First test copy ctor
	{
		const float init_val = 1.0f;
		ImageFx<float> imfx(2, 2, 1, init_val);
		Image<float> im(imfx);

		// Check that initial values were copied correctly
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], init_val);
		}

		// Check that imfx and im DO NOT share the data
		EXPECT_NE(imfx.raw(), im.raw());
		const float new_val = 2.0f;
		const Point p(1, 0);
		im(p) = new_val;
		EXPECT_NE(imfx(p), new_val);

		// Check that the metadata is the same
		EXPECT_EQ(imfx.size(), im.size());
		EXPECT_EQ(imfx.number_of_channels(), im.number_of_channels());
		EXPECT_EQ(imfx.color_space(), im.color_space());
	}

	// Then test assignment operator
	{
		const float init_val = 1.0f;
		ImageFx<float> imfx(2, 2, 1, init_val);
		Image<float> im;
		im = imfx;

		// Check that initial values were copied correctly
		const float *data = im.raw();
		for (int i = 0; i < im.raw_length(); ++i) {
			EXPECT_EQ(data[i], init_val);
		}

		// Check that imfx and im DO NOT share the data
		EXPECT_NE(imfx.raw(), im.raw());
		const float new_val = 2.0f;
		const Point p(0, 0);
		im(p) = new_val;
		EXPECT_NE(imfx(p), new_val);

		// Check that the metadata is the same
		EXPECT_EQ(imfx.size(), im.size());
		EXPECT_EQ(imfx.number_of_channels(), im.number_of_channels());
		EXPECT_EQ(imfx.color_space(), im.color_space());
	}
}


// ImageFx.clone() : data copying
TEST(Image, ImageFx_clone) {

	{
		const float init_val = 1.0f;
		ImageFx<float> im1(2, 2, 1, init_val);
		ImageFx<float> im2 = im1.clone();

		// Check that initial values were copied correctly
		const float *data = im2.raw();
		for (int i = 0; i < im2.raw_length(); ++i) {
			EXPECT_EQ(data[i], init_val);
		}

		// Check that im1 and im2 DO NOT share the data
		EXPECT_NE(im1.raw(), im2.raw());

		// Check that the metadata is the same
		EXPECT_EQ(im1.size(), im2.size());
		EXPECT_EQ(im1.number_of_channels(), im2.number_of_channels());
		EXPECT_EQ(im1.color_space(), im2.color_space());
	}
}


// Image.clone() : data copying
TEST(Image, Image_clone) {

	{
		const float init_val = 1.0f;
		Image<float> im1(2, 2, 1, init_val);
		Image<float> im2 = im1.clone();

		// Check that initial values were copied correctly
		const float *data = im2.raw();
		for (int i = 0; i < im2.raw_length(); ++i) {
			EXPECT_EQ(data[i], init_val);
		}

		// Check that im1 and im2 DO NOT share the data
		EXPECT_NE(im1.raw(), im2.raw());
		const float new_val = 2.0f;
		const Point p(1, 1);
		im1(p) = new_val;
		EXPECT_NE(im2(p), new_val);

		// Check that the metadata is the same
		EXPECT_EQ(im1.size(), im2.size());
		EXPECT_EQ(im1.number_of_channels(), im2.number_of_channels());
		EXPECT_EQ(im1.color_space(), im2.color_space());
	}
}


TEST(Image, ImageFx_access) {
	const int w = 4;
	const int h = 2;
	const int c = 3;
	Image<int> im(w, h, c, 0);

	int *data = im.raw();
	for (int i = 0; i < im.raw_length(); ++i) {
		data[i] = i;
	}

	ImageFx<int> imfx(im);

	// const T& operator() (...)
	{
		EXPECT_EQ(imfx(1, 0), 3);
		EXPECT_EQ(imfx(1, 0, 2), 5);
		EXPECT_EQ(imfx(Point(2, 1)), 18);
		EXPECT_EQ(imfx(Point(2, 1), 1), 19);
		EXPECT_NO_THROW(imfx(w, h));
		EXPECT_NO_THROW(imfx(w, h, c));
		EXPECT_NO_THROW(imfx(Point(w, h)));
		EXPECT_NO_THROW(imfx(Point(w, h), c));
	}

	// const T& at(...)
	{
		EXPECT_EQ(imfx.at(0, 1), 12);
		EXPECT_EQ(imfx.at(0, 1, 1), 13);
		EXPECT_EQ(imfx.at(Point(1, 1)), 15);
		EXPECT_EQ(imfx.at(Point(1, 1), 2), 17);
		EXPECT_THROW(imfx.at(w, h), std::out_of_range);
		EXPECT_THROW(imfx.at(w, h, c), std::out_of_range);
		EXPECT_THROW(imfx.at(Point(w, h)), std::out_of_range);
		EXPECT_THROW(imfx.at(Point(w, h), c), std::out_of_range);
	}

	// bool try_get_value(...)
	{
		int val = -1;
		EXPECT_TRUE(imfx.try_get_value(3, 0, val));
		EXPECT_EQ(val, 9);
		EXPECT_TRUE(imfx.try_get_value(3, 0, 1, val));
		EXPECT_EQ(val, 10);
		EXPECT_TRUE(imfx.try_get_value(Point(3, 1), val));
		EXPECT_EQ(val, 21);
		EXPECT_TRUE(imfx.try_get_value(Point(3, 1), 2, val));
		EXPECT_EQ(val, 23);
		EXPECT_FALSE(imfx.try_get_value(w, h, val));
		EXPECT_FALSE(imfx.try_get_value(w, h, c, val));
		EXPECT_FALSE(imfx.try_get_value(Point(w, h), val));
		EXPECT_FALSE(imfx.try_get_value(Point(w, h), c, val));
	}
}


TEST(Image, Image_const_access) {
	const int w = 4;
	const int h = 2;
	const int c = 3;
	Image<int> im(w, h, c, 0);

	int *data = im.raw();
	for (int i = 0; i < im.raw_length(); ++i) {
		data[i] = i;
	}

	const Image<int> im2(im);

	// const T& operator() (...)
	{
		EXPECT_EQ(im2(1, 0), 3);
		EXPECT_EQ(im2(1, 0, 2), 5);
		EXPECT_EQ(im2(Point(2, 1)), 18);
		EXPECT_EQ(im2(Point(2, 1), 1), 19);
		EXPECT_NO_THROW(im2(w, h));
		EXPECT_NO_THROW(im2(w, h, c));
		EXPECT_NO_THROW(im2(Point(w, h)));
		EXPECT_NO_THROW(im2(Point(w, h), c));
	}

	// const T& at(...)
	{
		EXPECT_EQ(im2.at(0, 1), 12);
		EXPECT_EQ(im2.at(0, 1, 1), 13);
		EXPECT_EQ(im2.at(Point(1, 1)), 15);
		EXPECT_EQ(im2.at(Point(1, 1), 2), 17);
		EXPECT_THROW(im2.at(w, h), std::out_of_range);
		EXPECT_THROW(im2.at(w, h, c), std::out_of_range);
		EXPECT_THROW(im2.at(Point(w, h)), std::out_of_range);
		EXPECT_THROW(im2.at(Point(w, h), c), std::out_of_range);
	}

	// bool try_get_value(...)
	{
		int val = -1;
		EXPECT_TRUE(im2.try_get_value(3, 0, val));
		EXPECT_EQ(val, 9);
		EXPECT_TRUE(im2.try_get_value(3, 0, 1, val));
		EXPECT_EQ(val, 10);
		EXPECT_TRUE(im2.try_get_value(Point(3, 1), val));
		EXPECT_EQ(val, 21);
		EXPECT_TRUE(im2.try_get_value(Point(3, 1), 2, val));
		EXPECT_EQ(val, 23);
		EXPECT_FALSE(im2.try_get_value(w, h, val));
		EXPECT_FALSE(im2.try_get_value(w, h, c, val));
		EXPECT_FALSE(im2.try_get_value(Point(w, h), val));
		EXPECT_FALSE(im2.try_get_value(Point(w, h), c, val));
	}
}


TEST(Image, Image_access) {
	const int w = 4;
	const int h = 2;
	const int c = 3;
	Image<int> im(w, h, c, 1);

	// T& operator() (...)
	{
		im(0, 0) = 3;
		EXPECT_EQ(im(0, 0), 3);
		im(1, 0, 2) = 4;
		EXPECT_EQ(im(1, 0, 2), 4);
		im(Point(2, 0)) = 5;
		EXPECT_EQ(im(Point(2, 0)), 5);
		im(Point(3, 0), 1) = 6;
		EXPECT_EQ(im(Point(3, 0), 1), 6);
		EXPECT_NO_THROW(im(w, h));
		EXPECT_NO_THROW(im(w, h, c));
		EXPECT_NO_THROW(im(Point(w, h)));
		EXPECT_NO_THROW(im(Point(w, h), c));
	}

	// T& at(...)
	{
		im.at(0, 1) = 7;
		EXPECT_EQ(im.at(0, 1), 7);
		im.at(0, 1, 1) = 8;
		EXPECT_EQ(im.at(0, 1, 1), 8);
		im.at(Point(2, 1)) = 9;
		EXPECT_EQ(im.at(Point(2, 1)), 9);
		im.at(Point(3, 1), 2) = 10;
		EXPECT_EQ(im.at(Point(3, 1), 2), 10);
		EXPECT_THROW(im.at(w, h), std::out_of_range);
		EXPECT_THROW(im.at(w, h, c), std::out_of_range);
		EXPECT_THROW(im.at(Point(w, h)), std::out_of_range);
		EXPECT_THROW(im.at(Point(w, h), c), std::out_of_range);
	}
}


TEST(Image, empty_access) {
	ImageFx<int> imfx;

	// const T& operator() (...)
	{
		EXPECT_NO_THROW(imfx(0, 0));
		EXPECT_NO_THROW(imfx(0, 0, 0));
		EXPECT_NO_THROW(imfx(Point(0, 0)));
		EXPECT_NO_THROW(imfx(Point(0, 0), 0));
	}

	// const T& at(...)
	{
		EXPECT_THROW(imfx.at(0, 0), std::out_of_range);
		EXPECT_THROW(imfx.at(0, 0, 0), std::out_of_range);
		EXPECT_THROW(imfx.at(Point(0, 0)), std::out_of_range);
		EXPECT_THROW(imfx.at(Point(0, 0), 0), std::out_of_range);
	}

	// bool try_get_value(...)
	{
		int val = -1;
		EXPECT_FALSE(imfx.try_get_value(0, 0, val));
		EXPECT_FALSE(imfx.try_get_value(0, 0, 0, val));
		EXPECT_FALSE(imfx.try_get_value(Point(0, 0), val));
		EXPECT_FALSE(imfx.try_get_value(Point(0, 0), 0, val));
	}

	Image<int> im;

	// T& operator() (...)
	{
		EXPECT_NO_THROW(im(0, 0));
		EXPECT_NO_THROW(im(0, 0, 0));
		EXPECT_NO_THROW(im(Point(0, 0)));
		EXPECT_NO_THROW(im(Point(0, 0), 0));
	}

	// T& at(...)
	{
		EXPECT_THROW(im.at(0, 0), std::out_of_range);
		EXPECT_THROW(im.at(0, 0, 0), std::out_of_range);
		EXPECT_THROW(im.at(Point(0, 0)), std::out_of_range);
		EXPECT_THROW(im.at(Point(0, 0), 0), std::out_of_range);
	}
}

