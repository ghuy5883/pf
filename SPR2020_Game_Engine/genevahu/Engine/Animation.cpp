#include "stdafx.h"
#include "Animation.h"
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"
#include <fstream>
#include <sstream>
#include "assetManager.h"

Animation::Animation(AssetManager* pAssetManager)
{
	m_AssetManager = pAssetManager;
	m_NumBones = NULL;
	m_NumFrames = NULL;
	m_AnimLength = NULL;
}

Animation::~Animation()
{
}

uint32_t Animation::GetNumBones() const
{
	return m_NumBones;
}

uint32_t Animation::GetNumFrames() const
{
	return m_NumFrames;
}

float Animation::GetLength() const
{
	return m_AnimLength;
}

Animation* Animation::StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager)
{
	Animation* pAnimation = new Animation(pAssetManager);
	if (false == pAnimation->Load(fileName))
	{
		delete pAnimation;
		return nullptr;
	}
	return pAnimation;
}

bool Animation::Load(const WCHAR* fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open()) {
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		return false;
	}

	std::string str = doc["metadata"]["type"].GetString();
	int ver = doc["metadata"]["version"].GetInt();

	// Check the metadata
	if (!doc["metadata"].IsObject() ||
		str != "itpanim" ||
		ver != 2)
	{
		return false;
	}

	// Get the sequence information
	m_NumFrames= doc["sequence"]["frames"].GetInt();
	m_AnimLength = doc["sequence"]["length"].GetDouble();
	m_NumBones = doc["sequence"]["bonecount"].GetInt();
	const rapidjson::Value& tracks = doc["sequence"]["tracks"];

	// Get bone transform information
	if (!tracks.IsArray() || tracks.Size() < 1) {
		return false;
	}

	// Populate the array of bone animation vector with a bunch of empty bone transform vectors;
	for (rapidjson::SizeType i = 0; i < m_NumBones; i++) {
		std::vector<BoneTransform> bonetransform;
		m_AnimData.emplace_back(bonetransform);
	}

	// Update the information in the bone transform vectors
	for (rapidjson::SizeType i = 0; i < tracks.Size(); i++) {	
		int bone = tracks[i]["bone"].GetInt();

		// Fetch bone transform information
		const rapidjson::Value& transforms = tracks[i]["transforms"];

		for (rapidjson::SizeType j = 0; j < transforms.Size(); j++) {
			BoneTransform transform;
			// Set rotation values
			transform.mRotation.x = transforms[j]["rot"][0].GetDouble();
			transform.mRotation.y = transforms[j]["rot"][1].GetDouble();
			transform.mRotation.z = transforms[j]["rot"][2].GetDouble();
			transform.mRotation.w = transforms[j]["rot"][3].GetDouble();

			// Set position values
			transform.mTranslation.x = transforms[j]["trans"][0].GetDouble();
			transform.mTranslation.y = transforms[j]["trans"][1].GetDouble();
			transform.mTranslation.z = transforms[j]["trans"][2].GetDouble();

			// Add the new transform with rotation and position values
			m_AnimData[bone].emplace_back(transform);
		}
	}


	return true;
}

void Animation::GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime) const
{
	// Get the keyframe closest to the current inTime
	float timePerFrame = GetLength() / (GetNumFrames() - 1);
	float timeRatio = inTime / m_AnimLength;
	int keyframeA = (int)(timeRatio * m_NumFrames);
	int keyframeB = (keyframeA + 1) % m_NumFrames;
	float fraction = (timeRatio * m_NumFrames) - keyframeA;

	// Loop through all bones to retrieve animation data for all bones
	for (uint32_t i = 0; i < m_NumBones; i++) {

		Matrix4 boneMatrix;
		// Check if this bone has tracks
		if (m_AnimData[i].size() > 0) {
			// It has tracks -- get the bone transform that is interpolated between keyframe a and b
			boneMatrix = BoneTransform::Interpolate(m_AnimData[i][keyframeA], m_AnimData[i][keyframeB], fraction).ToMatrix();
		}
		else {
			// It doesn't have tracks
			boneMatrix = Matrix4::Identity;
		}

		//Check if there is a parent matrix to combine with
		int parentIndex = inSkeleton->GetBone(i).mParentIndex;
		if (parentIndex != -1) {
			// Parent exists --> combine
			boneMatrix *= outPoses[parentIndex];
		}

		outPoses.emplace_back(boneMatrix);
	}
}
