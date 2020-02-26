#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32
#  ifdef PUBSUBUNMANAGED_API_EXPORTS
#    define PUBSUBUNMANAGED_API __declspec(dllexport)
#  else
#    define PUBSUBUNMANAGED_API __declspec(dllimport)
#  endif
#else
#  define PUBSUBUNMANAGED_API
#endif



	/// <summary>
	/// type declaration: callback for a screenshot request
	/// </summary>
	/// <remarks>
	/// value 1: the status of the request
	/// value 2: the size of the image data
	/// value 3: pointer to the image data
	/// </remarks>
	typedef void *LPVOID;
	typedef void (*publish_delegate)(int, int, LPVOID);
	//public delegate void mydelegate(int, int, void*);
	publish_delegate _publish_callback;


	PUBSUBUNMANAGED_API void read_test_image();

	/// <summary>
	/// vector contains 
	///</summary>
	std::vector<char> _image_data;

	PUBSUBUNMANAGED_API void check();

	PUBSUBUNMANAGED_API void SetSubscriber(publish_delegate callback);

#ifdef __cplusplus
}
#endif
