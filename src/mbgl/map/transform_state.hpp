#pragma once

#include <mbgl/map/camera.hpp>
#include <mbgl/map/mode.hpp>
#include <mbgl/util/camera.hpp>
#include <mbgl/util/constants.hpp>
#include <mbgl/util/geo.hpp>
#include <mbgl/util/geometry.hpp>
#include <mbgl/util/mat4.hpp>
#include <mbgl/util/projection.hpp>
#include <mbgl/util/size.hpp>

#include <cstdint>
#include <array>
#include <limits>
#include <optional>

namespace mbgl {

class UnwrappedTileID;
class TileCoordinate;

struct TransformStateProperties {
    TransformStateProperties& withX(const std::optional<double>& val) {
        x = val;
        return *this;
    }
    TransformStateProperties& withY(const std::optional<double>& val) {
        y = val;
        return *this;
    }
    TransformStateProperties& withScale(const std::optional<double>& val) {
        scale = val;
        return *this;
    }
    TransformStateProperties& withBearing(const std::optional<double>& val) {
        bearing = val;
        return *this;
    }
    TransformStateProperties& withPitch(const std::optional<double>& val) {
        pitch = val;
        return *this;
    }
    TransformStateProperties& withXSkew(const std::optional<double>& val) {
        xSkew = val;
        return *this;
    }
    TransformStateProperties& withYSkew(const std::optional<double>& val) {
        ySkew = val;
        return *this;
    }
    TransformStateProperties& withAxonometric(const std::optional<bool>& val) {
        axonometric = val;
        return *this;
    }
    TransformStateProperties& withPanningInProgress(const std::optional<bool>& val) {
        panning = val;
        return *this;
    }
    TransformStateProperties& withScalingInProgress(const std::optional<bool>& val) {
        scaling = val;
        return *this;
    }
    TransformStateProperties& withRotatingInProgress(const std::optional<bool>& val) {
        rotating = val;
        return *this;
    }
    TransformStateProperties& withEdgeInsets(const std::optional<EdgeInsets>& val) {
        edgeInsets = val;
        return *this;
    }
    TransformStateProperties& withSize(const std::optional<Size>& val) {
        size = val;
        return *this;
    }
    TransformStateProperties& withConstrainMode(const std::optional<ConstrainMode>& val) {
        constrain = val;
        return *this;
    }
    TransformStateProperties& withNorthOrientation(const std::optional<NorthOrientation>& val) {
        northOrientation = val;
        return *this;
    }
    TransformStateProperties& withViewportMode(const std::optional<ViewportMode>& val) {
        viewPortMode = val;
        return *this;
    }

    std::optional<double> x;
    std::optional<double> y;
    std::optional<double> bearing;
    std::optional<double> scale;
    std::optional<double> pitch;
    std::optional<double> xSkew;
    std::optional<double> ySkew;
    std::optional<bool> axonometric;
    std::optional<bool> panning;
    std::optional<bool> scaling;
    std::optional<bool> rotating;
    std::optional<EdgeInsets> edgeInsets;
    std::optional<Size> size;
    std::optional<ConstrainMode> constrain;
    std::optional<NorthOrientation> northOrientation;
    std::optional<ViewportMode> viewPortMode;
};

class TransformState {
public:
    TransformState(ConstrainMode = ConstrainMode::HeightOnly, ViewportMode = ViewportMode::Default);

    void setProperties(const TransformStateProperties& properties);

    // Matrix
    void matrixFor(mat4&, const UnwrappedTileID&) const;
    void getProjMatrix(mat4& matrix, uint16_t nearZ = 1, bool aligned = false) const;

    // Dimensions
    Size getSize() const;
    void setSize(const Size& size_);

    // North Orientation
    NorthOrientation getNorthOrientation() const;
    double getNorthOrientationAngle() const;
    void setNorthOrientation(NorthOrientation);

    // Constrain mode
    ConstrainMode getConstrainMode() const;
    void setConstrainMode(ConstrainMode);

    // Viewport mode
    ViewportMode getViewportMode() const;
    void setViewportMode(ViewportMode val);

    CameraOptions getCameraOptions(const std::optional<EdgeInsets>&) const;

    // EdgeInsects
    EdgeInsets getEdgeInsets() const { return edgeInsets; }
    void setEdgeInsets(const EdgeInsets&);

    // Position
    LatLng getLatLng(LatLng::WrapMode = LatLng::Unwrapped) const;
    double pixel_x() const;
    double pixel_y() const;

    // Zoom
    double getZoom() const;
    uint8_t getIntegerZoom() const;
    double getZoomFraction() const;

    // Scale
    double getScale() const;
    void setScale(double);

    // Positions
    double getX() const;
    void setX(double);
    double getY() const;
    void setY(double);

    // Bounds
    void setLatLngBounds(LatLngBounds);
    LatLngBounds getLatLngBounds() const;
    void setMinZoom(double);
    double getMinZoom() const;
    void setMaxZoom(double);
    double getMaxZoom() const;
    void setMinPitch(double);
    double getMinPitch() const;
    void setMaxPitch(double);
    double getMaxPitch() const;

    // Rotation
    double getBearing() const;
    void setBearing(double);
    float getFieldOfView() const;
    float getCameraToCenterDistance() const;
    double getPitch() const;
    void setPitch(double);

    double getXSkew() const;
    void setXSkew(double);
    double getYSkew() const;
    void setYSkew(double);
    bool getAxonometric() const;
    void setAxonometric(bool);

    // State
    bool isChanging() const;
    bool isRotating() const;
    void setRotatingInProgress(bool val) { rotating = val; }
    bool isScaling() const;
    void setScalingInProgress(bool val) { scaling = val; }
    bool isPanning() const;
    void setPanningInProgress(bool val) { panning = val; }
    bool isGestureInProgress() const;
    void setGestureInProgress(bool val) { gestureInProgress = val; }

    // Conversion
    ScreenCoordinate latLngToScreenCoordinate(const LatLng&) const;
    ScreenCoordinate latLngToScreenCoordinate(const LatLng&, vec4&) const;
    LatLng screenCoordinateToLatLng(const ScreenCoordinate&, LatLng::WrapMode = LatLng::Unwrapped) const;
    // Implements mapbox-gl-js pointCoordinate() : MercatorCoordinate.
    TileCoordinate screenCoordinateToTileCoordinate(const ScreenCoordinate&, uint8_t atZoom) const;

    double zoomScale(double zoom) const;
    double scaleZoom(double scale) const;

    bool valid() const { return !size.isEmpty() && (scale >= min_scale && scale <= max_scale); }

    float getCameraToTileDistance(const UnwrappedTileID&) const;
    float maxPitchScaleFactor() const;

    /** Recenter the map so that the given coordinate is located at the given
        point on screen. */
    void moveLatLng(const LatLng&, const ScreenCoordinate&);
    void setLatLngZoom(const LatLng& latLng, double zoom);

    void constrain(double& scale, double& x, double& y) const;
    bool constrainScreen(double& scale_, double& x_, double& y_) const;
    void constrainCameraAndZoomToBounds(CameraOptions& camera, double& zoom) const;

    const mat4& getProjectionMatrix() const;
    const mat4& getInvProjectionMatrix() const;

    FreeCameraOptions getFreeCameraOptions() const;
    void setFreeCameraOptions(const FreeCameraOptions& options);

private:
    bool rotatedNorth() const;

    // Viewport center offset, from [size.width / 2, size.height / 2], defined
    // by |edgeInsets| in screen coordinates, with top left origin.
    ScreenCoordinate getCenterOffset() const;

    LatLngBounds bounds;

    // Limit the amount of zooming possible on the map.
    double min_scale = std::pow(2, 0);
    double max_scale = std::pow(2, util::DEFAULT_MAX_ZOOM);

    // Limit the amount of pitch
    double minPitch = util::PITCH_MIN;
    double maxPitch = util::PITCH_MAX;

    NorthOrientation orientation = NorthOrientation::Upwards;

    // logical dimensions
    Size size;

    mat4 coordinatePointMatrix(const mat4& projMatrix) const;
    mat4 getPixelMatrix() const;

    void setScalePoint(double scale, const ScreenCoordinate& point);

    void updateMatricesIfNeeded() const;
    bool needsMatricesUpdate() const { return requestMatricesUpdate; }

    bool setCameraPosition(const vec3& position);
    bool setCameraOrientation(const Quaternion& orientation);
    void updateCameraState() const;
    void updateStateFromCamera();

    const mat4& getCoordMatrix() const;
    const mat4& getInvertedMatrix() const;

private:
    ConstrainMode constrainMode;
    ViewportMode viewportMode;

    // animation state
    bool rotating = false;
    bool scaling = false;
    bool panning = false;
    bool gestureInProgress = false;

    // map position
    double x = 0, y = 0;
    double bearing = 0;
    double scale = 1;
    // This fov value is somewhat arbitrary. The altitude of the camera used
    // to be defined as 1.5 screen heights above the ground, which was an
    // arbitrary choice. This is the fov equivalent to that value calculated
    // with: `fov = 2 * arctan((height / 2) / (height * 1.5))`
    double fov = 0.6435011087932844;
    double pitch = 0.0;
    double xSkew = 0.0;
    double ySkew = 1.0;
    bool axonometric = false;

    EdgeInsets edgeInsets;
    mutable util::Camera camera;

    // cache values for spherical mercator math
    double Bc = Projection::worldSize(scale) / util::DEGREES_MAX;
    double Cc = Projection::worldSize(scale) / util::M2PI;

    mutable bool requestMatricesUpdate{true};
    mutable mat4 projectionMatrix;
    mutable mat4 invProjectionMatrix;
    mutable mat4 coordMatrix;
    mutable mat4 invertedMatrix;
};

} // namespace mbgl
