
#include "Exception.h"


namespace libthing {

class LayerException : public libthing::Exception {
	public: LayerException(const char *msg): libthing::Exception(msg) {	};
};


/// A tape measure for layers, since layers have variable height.
/// Class that relates height (a scalar) to layer index (unsigned int)
//
/// This class assumes that the model's triangles are
/// all above 0 (the z of each of the 3 vertices is >= 0.0).
/// worse, the layers MUST start at 0. Lazy programmer!
/// This is good enough for now, until the class "sees" every triangle
/// during loading and recalcs layers on the fly.


class LayerMeasure
{
	Scalar firstLayerZ;
	Scalar layerH;
	//index_t maxLayerIndex;

public:
	LayerMeasure(Scalar firstLayerZ, Scalar layerH)
		:firstLayerZ(firstLayerZ), layerH(layerH)
	{}




	index_t zToLayerAbove(Scalar z) const
	{
		if(z < 0) {
			LayerException mixup("Model with points below the z axis are not supported in this version. Please center your model on the build area");
			throw mixup;
		}

		if (z < firstLayerZ)
			return 0;

		Scalar tol = 0.00000000000001; // tolerance
		Scalar layer = (z -firstLayerZ + tol ) / layerH;
		return (unsigned int)ceil(layer) ;
	}

	Scalar sliceIndexToHeight(unsigned int sliceIndex) const
	{
		return firstLayerZ + sliceIndex * layerH;
	}

	Scalar getLayerH() const
	{
		return layerH;
	}
};

}; //end namespace
