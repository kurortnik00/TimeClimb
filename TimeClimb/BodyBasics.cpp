
//------------------------------------------------------------------------------
// <copyright file="BodyBasics.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------
#include <strsafe.h>
#include "BodyBasics.h"
#include <iostream>

using namespace std;

static const float c_JointThickness = 3.0f;
static const float c_TrackedBoneThickness = 6.0f;
static const float c_InferredBoneThickness = 1.0f;
static const float c_HandSize = 30.0f;


template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}
/*
int main()
{
	CBodyBasics application;
	application.Run();
}*/

/// <summary>
/// Constructor
/// </summary>
CBodyBasics::CBodyBasics() :
	m_hWnd(NULL),
	m_nStartTime(0),
	m_nLastCounter(0),
	m_nFramesSinceUpdate(0),
	m_fFreq(0),
	m_nNextStatusTime(0LL),
	m_pKinectSensor(NULL),
	m_pCoordinateMapper(NULL),
	m_pBodyFrameReader(NULL)
{
	LARGE_INTEGER qpf = { 0 };
	if (QueryPerformanceFrequency(&qpf))
	{
		m_fFreq = double(qpf.QuadPart);
	}

}


/// <summary>
/// Destructor
/// </summary>
CBodyBasics::~CBodyBasics()
{

	// done with body frame reader
	SafeRelease(m_pBodyFrameReader);

	// done with coordinate mapper
	SafeRelease(m_pCoordinateMapper);

	// close the Kinect Sensor
	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();
	}

	SafeRelease(m_pKinectSensor);
}

void CBodyBasics::Run()
{
	InitializeDefaultSensor();
}


void CBodyBasics::Update()
{
	if (!m_pBodyFrameReader)
	{
		return;
	}
	IBodyFrame* pBodyFrame = NULL;

	HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

	if (SUCCEEDED(hr))
	{
		INT64 nTime = 0;

		hr = pBodyFrame->get_RelativeTime(&nTime);

		IBody* ppBodies[BODY_COUNT] = { 0 };

		if (SUCCEEDED(hr))
		{
			hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
		}

		if (SUCCEEDED(hr))
		{
			ProcessBody(nTime, BODY_COUNT, ppBodies);
		}

		for (int i = 0; i < _countof(ppBodies); ++i)
		{
			SafeRelease(ppBodies[i]);
		}
	}

	SafeRelease(pBodyFrame);
}


HRESULT CBodyBasics::InitializeDefaultSensor()
{
	HRESULT hr;

	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr))
	{
		return hr;

	}

	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get coordinate mapper and the body reader
		IBodyFrameSource* pBodyFrameSource = NULL;

		hr = m_pKinectSensor->Open();

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
		}

		SafeRelease(pBodyFrameSource);
	}

	if (!m_pKinectSensor || FAILED(hr))
	{
		cout << "No ready Kinect found!";
		return E_FAIL;
	}

	return hr;
}

void CBodyBasics::ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies)
{
	HRESULT hr;

	if (m_pCoordinateMapper)
	{
		int width = 640;
		int height = 280;

		for (int i = 0; i < nBodyCount; ++i)
		{
			IBody* pBody = ppBodies[i];
			if (pBody)
			{
				BOOLEAN bTracked = false;
				hr = pBody->get_IsTracked(&bTracked);

				if (bTracked)
				{
					Joint joints[JointType_Count];
					D2D1_POINT_2F jointPoints[JointType_Count];
					//TrackingConfidence leftHandConfidence;
					//TrackingConfidence rightHandConfidence;

					//pBody->get_HandRightConfidence(&leftHandConfidence);
					//pBody->get_HandRightConfidence(&rightHandConfidence);

					hr = pBody->GetJoints(_countof(joints), joints);
					pBody->GetJoints(_countof(trackPoints), trackPoints);
					if (SUCCEEDED(hr))
					{
						for (int j = 0; j < _countof(joints); ++j)
						{
							jointPoints[j] = BodyToScreen(joints[j].Position, width, height);
							trackPointsXY[j] = jointPoints[j];
							trackDepthPoint[j] = joints[j].Position.Z;



							//cout << rightHandConfidence;
							//cout << "\n";
						}
					}
				}
			}
		}
	}
}


D2D1_POINT_2F CBodyBasics::BodyToScreen(const CameraSpacePoint& bodyPoint, int width, int height)
{
	// Calculate the body's position on the screen
	DepthSpacePoint depthPoint = { 0 };
	m_pCoordinateMapper->MapCameraPointToDepthSpace(bodyPoint, &depthPoint);

	float screenPointX = static_cast<float>(depthPoint.X * width) / cDepthWidth;
	float screenPointY = static_cast<float>(depthPoint.Y * height) / cDepthHeight;

	return D2D1::Point2F(screenPointX, screenPointY);
}



D2D1_POINT_2F CBodyBasics::SkeletPointsXY(int i)
{
	Update();
	return trackPointsXY[i];
}

float CBodyBasics::DepthSkeletonPoints(int i)
{
	Update();
	return trackDepthPoint[i];
}



sf::Vector2f CBodyBasics::arms_legs_pointAveraged_PointsXY(int limb)
{
	Update();
	switch (limb)
	{
	case CBodyBasics::RIGHT_ARM:
		return sf::Vector2f((trackPointsXY[HANDRIGHT].x + trackPointsXY[WRISTRIGHT].x + trackPointsXY[HANDTIPRIGHT].x + trackPointsXY[THUMBRIGHT].x) / 4,
			(trackPointsXY[HANDRIGHT].y + trackPointsXY[WRISTRIGHT].y + trackPointsXY[HANDTIPRIGHT].y + trackPointsXY[THUMBRIGHT].y) / 4);
		break;
	case CBodyBasics::LEFT_ARM:
		return sf::Vector2f((trackPointsXY[HANDLEFT].x + trackPointsXY[WRISTLEFT].x + trackPointsXY[HANDTIPLEFT].x + trackPointsXY[THUMBLEFT].x) / 4,
			(trackPointsXY[HANDLEFT].y + trackPointsXY[WRISTLEFT].y + trackPointsXY[HANDTIPLEFT].y + trackPointsXY[THUMBLEFT].y) / 4);
		break;
	case CBodyBasics::RIGHT_LEG:
		return sf::Vector2f((trackPointsXY[FOOTRIGHT].x + trackPointsXY[ANKLERIGHT].x) / 2,
			(trackPointsXY[FOOTRIGHT].y + trackPointsXY[ANKLERIGHT].y) / 2);
		break;
	case CBodyBasics::LEFT_LEG:
		return sf::Vector2f((trackPointsXY[FOOTRIGHT].x + trackPointsXY[ANKLERIGHT].x) / 2,
			(trackPointsXY[FOOTRIGHT].y + trackPointsXY[WRISTLEFT].y / 2));
		break;
	default:
		return sf::Vector2f(-1, -1);
		break;
	}
}


float CBodyBasics::arms_legs_pointAveraged_DepthPoints(int limb)
{
	Update();
	switch (limb)
	{
	case CBodyBasics::RIGHT_ARM:
		return (trackDepthPoint[HANDRIGHT]
			+ trackDepthPoint[WRISTRIGHT]
			+ trackDepthPoint[HANDTIPRIGHT]
			+ trackDepthPoint[THUMBRIGHT]
			+ trackDepthPoint[ELBOWRIGHT]) / 5;
		break;
	case CBodyBasics::LEFT_ARM:
		return (trackDepthPoint[HANDLEFT]
			+ trackDepthPoint[WRISTLEFT]
			+ trackDepthPoint[HANDTIPLEFT]
			+ trackDepthPoint[THUMBLEFT]
			+ trackDepthPoint[ELBOWLEFT]) / 5;
		break;
	case CBodyBasics::RIGHT_LEG:
		return (trackDepthPoint[KNEERIGHT]
			+ trackDepthPoint[ANKLERIGHT]
			+ trackDepthPoint[FOOTRIGHT]) / 3;
		break;
	case CBodyBasics::LEFT_LEG:
		return (trackDepthPoint[KNEELEFT]
			+ trackDepthPoint[ANKLELEFT]
			+ trackDepthPoint[FOOTLEFT]) / 3;
		break;
	default:
		break;
	}
}


sf::Vector2f CBodyBasics::allJoints_timeAveraged_PointsXY(int limb)
{
	sf::Vector2f timeAveraged_Point[JointType_Count];
	Update();
	JointPoints_buffer jointPoints;
	for (int i = 0; i < JointType_Count; i++)
	{
		jointPoints.joints[i] = trackPointsXY[i];
	}

	buffer.push_back(jointPoints);
	if (buffer.size() > 5) buffer.erase(buffer.begin());
	int flag = 0;
	for (auto &i : buffer)
	{
		for (int j = 0; j < JointType_Count; j++)
		{
			if (flag == 0)
			{

				timeAveraged_Point[j].x = i.joints[j].x;
				timeAveraged_Point[j].y = i.joints[j].y;
			}
			else
			{
				timeAveraged_Point[j].x += i.joints[j].x;
				timeAveraged_Point[j].y += i.joints[j].y;
			}
		}
		flag = 1;
	}
	for (int j = 0; j < JointType_Count; j++)
	{
		timeAveraged_Point[j].x = timeAveraged_Point[j].x / buffer.size();
		timeAveraged_Point[j].y = timeAveraged_Point[j].y / buffer.size();
	}

	return timeAveraged_Point[limb];

}

float CBodyBasics::allJoints_timeAveraged_DepthPoints(int limb)
{
	float timeAveraged_DepthPoint[JointType_Count];
	Update();
	JointPoints_Depthbuffer jointDepthPoints;
	for (int i = 0; i < JointType_Count; i++)
	{
		jointDepthPoints.jointsDepth[i] = trackDepthPoint[i];
	}
	depthBuffer.push_back(jointDepthPoints);
	if (depthBuffer.size() > 5) depthBuffer.erase(depthBuffer.begin());
	int flag = 0;
	for (auto &i : depthBuffer)
	{
		for (int j = 0; j < JointType_Count; j++)
		{
			if (flag == 0)
			{

				timeAveraged_DepthPoint[j] = i.jointsDepth[j];
			}
			else
			{
				timeAveraged_DepthPoint[j] += i.jointsDepth[j];
			}
		}
		flag = 1;
	}
	for (int j = 0; j < JointType_Count; j++)
	{
		timeAveraged_DepthPoint[j] = timeAveraged_DepthPoint[j] / depthBuffer.size();
	}

	return timeAveraged_DepthPoint[limb];
}


sf::Vector2f CBodyBasics::arms_legs_timeAveraged_PointsXY(int limb)
{

	sf::Vector2f timeAveraged_Point[LEFT_LEG + 1];
	Update();
	JointPoints_vec_buffer vec_b;
	for (int i = 0; i < LEFT_LEG + 1; i++)
	{
		vec_b.joints[i] = arms_legs_pointAveraged_PointsXY(i);
	}

	vec_Buffer.push_back(vec_b);
	if (vec_Buffer.size() > 5) vec_Buffer.erase(vec_Buffer.begin());
	int flag = 0;
	for (auto &i : vec_Buffer)
	{
		for (int j = 0; j < LEFT_LEG + 1; j++)
		{
			if (flag == 0)
			{

				timeAveraged_Point[j].x = i.joints[j].x;
				timeAveraged_Point[j].y = i.joints[j].y;
			}
			else
			{
				timeAveraged_Point[j].x += i.joints[j].x;
				timeAveraged_Point[j].y += i.joints[j].y;
			}
		}
		flag = 1;
	}
	for (int j = 0; j < LEFT_LEG + 1; j++)
	{
		timeAveraged_Point[j].x = timeAveraged_Point[j].x / vec_Buffer.size();
		timeAveraged_Point[j].y = timeAveraged_Point[j].y / vec_Buffer.size();
	}

	return timeAveraged_Point[limb];

}



float CBodyBasics::arms_legs_timeAveraged_DepthPoints(int limb)
{
	float timeAveraged_DepthPoint[LEFT_LEG + 1];
	Update();
	JointPoints_Depthbuffer jointDepthPoints;
	for (int i = 0; i < LEFT_LEG + 1; i++)
	{
		jointDepthPoints.jointsDepth_4[i] = arms_legs_pointAveraged_DepthPoints(i);
	}
	depthBuffer.push_back(jointDepthPoints);
	if (depthBuffer.size() > 5) depthBuffer.erase(depthBuffer.begin());
	int flag = 0;
	for (auto &i : depthBuffer)
	{
		for (int j = 0; j < LEFT_LEG + 1; j++)
		{
			if (flag == 0)
			{

				timeAveraged_DepthPoint[j] = i.jointsDepth_4[j];
			}
			else
			{
				timeAveraged_DepthPoint[j] += i.jointsDepth_4[j];
			}
		}
		flag = 1;
	}
	for (int j = 0; j < LEFT_LEG + 1; j++)
	{
		timeAveraged_DepthPoint[j] = timeAveraged_DepthPoint[j] / depthBuffer.size();
	}

	return timeAveraged_DepthPoint[limb];
}