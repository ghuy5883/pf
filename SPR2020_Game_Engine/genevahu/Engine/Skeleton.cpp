#include "stdafx.h"
#include "Skeleton.h"
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"
#include <fstream>
#include <sstream>
#include "assetManager.h"

Skeleton::Skeleton(AssetManager* pAssetManager)
{
	mAssetManager = pAssetManager;
}

Skeleton::~Skeleton()
{
}

size_t Skeleton::GetNumBones() const
{
	return mBones.size();
}

const Skeleton::Bone& Skeleton::GetBone(size_t idx) const
{
	return mBones[idx];
}

const std::vector<Skeleton::Bone>& Skeleton::GetBones() const
{
	return mBones;
}

const std::vector<Matrix4>& Skeleton::GetGlobalInvBindPoses() const
{
	return mGlobalInverseBindPoseMatrices;
}

Skeleton* Skeleton::StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager)
{
	Skeleton* pSkeleton = new Skeleton(pAssetManager);
	if (false == pSkeleton->Load(fileName))
	{
		delete pSkeleton;
		return nullptr;
	}
	return pSkeleton;
}

bool Skeleton::Load(const WCHAR* fileName)
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
		str != "itpskel" ||
		ver != 1)
	{
		return false;
	}

	// Get bonecount
	int boneCount = doc["bonecount"].GetInt();

	// Get bones array
	const rapidjson::Value& bones = doc["bones"];
	if (!bones.IsArray() || bones.Size() < 1) {
		return false;
	}
	for (rapidjson::SizeType i = 0; i < bones.Size(); i++) {
		std::string boneName = bones[i]["name"].GetString();
		int boneParent = bones[i]["parent"].GetInt();
		const rapidjson::Value& bindpose = bones[i]["bindpose"];

		const rapidjson::Value& rot = bindpose["rot"];
		// Stuff all rotation values into a quaternion
		Quaternion rotation = Quaternion(rot[0].GetDouble(), rot[1].GetDouble(), rot[2].GetDouble(), rot[3].GetDouble());
		
		const rapidjson::Value& trans = bindpose["trans"];
		// Stuff all transform values into a vector3
		Vector3 transform = Vector3(trans[0].GetDouble(), trans[1].GetDouble(), trans[2].GetDouble());

		// Make a bone with the provided data
		Skeleton::Bone newBone;
		newBone.mBoneTransform.mRotation = rotation;
		newBone.mBoneTransform.mTranslation = transform;
		newBone.mName = boneName;
		newBone.mParentIndex = boneParent;

		// Add the newly made bone to the list of bones
		mBones.emplace_back(newBone);
	}

	// All bones loaded in, compute global inverse bind pose.
	ComputeGlobalInvBindPose();

	return true;
}

void Skeleton::ComputeGlobalInvBindPose()
{
	// Loop through all the bones, calculating the bind pose of each
	for (int i = 0; i < mBones.size(); i++) {
		// If the bone has a parent, multiply bind pose of child bone by its parent bone
		Matrix4 currBindPose = mBones[i].mBoneTransform.ToMatrix();
		Matrix4 parentBindPose;
		if (mBones[i].mParentIndex != -1) {
			// Get the parent's bind pose matrix
			parentBindPose = mGlobalInverseBindPoseMatrices[mBones[i].mParentIndex];
		}
		else {
			parentBindPose = Matrix4::Identity;
		}

		mGlobalInverseBindPoseMatrices.push_back(currBindPose * parentBindPose);
	}
	
	// Loop through all the bones, inverting the bind pose of each
	for (int i = 0; i < mGlobalInverseBindPoseMatrices.size(); i++) {
		mGlobalInverseBindPoseMatrices[i].Invert();
	}
}
