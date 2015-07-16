#ifndef _LIVE_DASH_IMAGE_SOURCE_H_
#define _LIVE_DASH_IMAGE_SOURCE_H_

#include "Plugin.h"
#include "LiveDash.h"

class CLiveDashImageSource : public ImageSource
{
public:
	CLiveDashImageSource(CLiveDashboard* dash);
	~CLiveDashImageSource();

	virtual void Render(const Vect2 &pos, const Vect2 &size);
	virtual Vect2 GetSize() const;

private:
	CLiveDashboard* _ld;
	VertexBuffer* _renderPlane;

	// this texture contains a static image of the dashboard and is used when no
	// rendering is needed. eg when car is in garage or no sim is connected!
	Texture* _staticTexture;

	// the current rendered texture
	Texture* _currentTexture;

	ImageInfo _imgInfo;
};


#endif // _LIVE_DASH_IMAGE_SOURCE_H_