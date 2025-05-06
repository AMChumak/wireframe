//
// Created by anton on 5/6/25.
//

#include "ConfigKeeper.h"

#include <fstream>
#include <nlohmann/json.hpp>

using nlohmann::json;

ConfigKeeper::ConfigKeeper()
{
    M = 2;
    M1 = 1;
    cameraZoom = 10;
    cameraCenter = {0, 0, 0};
    rotation = Matrix4d::Identity();
    zForward = 9;
    spline.addKeyPoint(Point3D{0, -1, 0});
    spline.addKeyPoint(Point3D{1, -1, 0});
    spline.addKeyPoint(Point3D{2, -1, 0});
    spline.addKeyPoint(Point3D{3, -1, 0});
}


void to_json(json& j, const Point3D& p)
{
    j = json{
        {"x", p.x},
        {"y", p.y},
        {"z", p.z}
    };
}

void from_json(const json& j, Point3D& p)
{
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
    j.at("z").get_to(p.z);
}


std::string ConfigKeeper::saveConfig(const std::string& fileName)
{
    json config;

    config["M"] = M;
    config["M1"] = M1;
    config["zoom"] = cameraZoom;
    config["cameraCenter"] = cameraCenter;
    config["splineKeyPoints"] = spline.keyPoints();
    config["splineCntParts"] = spline.getCntParts();
    config["zf"] = zForward;
    config["rotation"] = std::vector<double>(rotation.data(), rotation.data() + rotation.size());

    try
    {
        std::ofstream file(fileName);
        file << config.dump(4);
    }
    catch (const std::exception& e)
    {
        std::stringstream sstream;
        sstream << e.what();
        return sstream.str();
    }
    return "";
}

std::string ConfigKeeper::loadConfig(const std::string& fileName)
{
    try
    {
        std::ifstream config_file(fileName);
        json config = json::parse(config_file);

        if (!config.contains("M") || !config.contains("M1") || !config.contains("zoom") || !config.
            contains("cameraCenter") || !config.contains("splineKeyPoints") || !config.contains("splineCntParts") || !
            config.contains("zf") || !config.contains("rotation"))
        {
            return "There is not all fields! (M, M1, zoom, cameraCenter, splineKeyPoints, splineCntParts, zf, rotation) ";
        }

        M = config["M"].get<int>();
        M1 = config["M1"].get<int>();
        cameraZoom = config["zoom"].get<double>();
        cameraCenter = config["cameraCenter"].get<Point3D>();
        spline.setKeyPoints(config["splineKeyPoints"].get<std::vector<Point3D>>());
        spline.setCntParts(config["splineCntParts"].get<int>());
        zForward = config["zf"].get<double>();
        std::vector<double> rotationV = config["rotation"].get<std::vector<double>>();
        rotation = Eigen::Map<Eigen::Matrix4d>(rotationV.data(), 4, 4);

    }
    catch (const std::exception& e)
    {
        std::stringstream sstream;
        sstream << "Error while reading config: " << e.what() << std::endl;
        return sstream.str();
    }
    return "";
}
