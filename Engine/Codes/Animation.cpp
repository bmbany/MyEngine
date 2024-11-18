#include "Animation.h"

void Engine::Animation::LoadAnimation(const aiScene* paiScene)
{
	auto AssimpVec3ToSimpleMathVec3 = [](const aiVector3D& in) { return XMVectorSet(in.x, in.y, in.z, 0.f); };
	auto AssimpQuatToSimpleMathQuat = [](const aiQuaternion& in) { return XMVectorSet(in.x, in.y, in.z, in.w); };

	for (unsigned int i = 0; i < paiScene->mNumAnimations; i++)
	{
		Channel animation;
		aiAnimation* anim = paiScene->mAnimations[i];

		if (anim->mTicksPerSecond != 0.f)
			animation.ticksPerSecond = (float)anim->mTicksPerSecond;

		animation.duration = float(anim->mDuration * anim->mTicksPerSecond);
		
		for (unsigned int j = 0; j < anim->mNumChannels; j++)
		{
			aiNodeAnim* channel = anim->mChannels[j];
			BoneTransformTrack track;

			for (unsigned int k = 0; k < channel->mNumPositionKeys; k++)
			{				
				track.positions.emplace_back(
					(float)channel->mPositionKeys[k].mTime,
					AssimpVec3ToSimpleMathVec3(channel->mPositionKeys[k].mValue));
			}

			for (unsigned int k = 0; k < channel->mNumRotationKeys; k++)
			{
				track.rotations.emplace_back(
					(float)channel->mRotationKeys[k].mTime,
					AssimpQuatToSimpleMathQuat(channel->mRotationKeys[k].mValue));
			}

			for (unsigned int k = 0; k < channel->mNumScalingKeys; k++)
			{
				track.scales.emplace_back(
					(float)channel->mScalingKeys[k].mTime,
					AssimpVec3ToSimpleMathVec3(channel->mScalingKeys[k].mValue));
			}

			animation.boneTransforms[channel->mNodeName.C_Str()] = track;
			animation.lastTime = max(track.scales.back().first, track.rotations.back().first);
			animation.lastTime = max(animation.lastTime, track.positions.back().first);
		}

		_animations[anim->mName.C_Str()] = animation;
	}
}

HRESULT Engine::Animation::LoadResource(const std::filesystem::path& filePath)
{
	return S_OK;
}

void Engine::Animation::Free()
{
}