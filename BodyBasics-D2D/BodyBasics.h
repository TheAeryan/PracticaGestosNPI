//------------------------------------------------------------------------------
// <copyright file="BodyBasics.h" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#pragma once

#include "stdafx.h"
#include "resource.h"
#include "ApiGestos.h"


#include <GL/glew.h>
#include <GLFW/glfw3.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A body basics. </summary>
///
/// <remarks>	Mike, 19/12/2019. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class CBodyBasics
{
    /// <summary>	Width of the depth. </summary>
    static const int        cDepthWidth  = 512;
    /// <summary>	Height of the depth. </summary>
    static const int        cDepthHeight = 424;
public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Constructor. </summary>
    ///
    /// <remarks>	Mike, 19/12/2019. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    CBodyBasics();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Destructor. </summary>
    ///
    /// <remarks>	Mike, 19/12/2019. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ~CBodyBasics();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Handles window messages, passes most to the class instance to handle. </summary>
    ///
    /// <remarks>	Mike, 19/12/2019. </remarks>
    ///
    /// <param name="hWnd">  	window message is for. </param>
    /// <param name="uMsg">  	message. </param>
    /// <param name="wParam">	message data. </param>
    /// <param name="lParam">	additional message data. </param>
    ///
    /// <returns>	result of message processing. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Handle windows messages for a class instance. </summary>
    ///
    /// <remarks>	Mike, 19/12/2019. </remarks>
    ///
    /// <param name="hWnd">  	window message is for. </param>
    /// <param name="uMsg">  	message. </param>
    /// <param name="wParam">	message data. </param>
    /// <param name="lParam">	additional message data. </param>
    ///
    /// <returns>	result of message processing. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LRESULT CALLBACK        DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Creates the main window and begins processing. </summary>
    ///
    /// <remarks>	Mike, 19/12/2019. </remarks>
    ///
    /// <param name="hInstance">	. </param>
    /// <param name="nCmdShow"> 	. </param>
    ///
    /// <returns>	An int. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    int                     Run(HINSTANCE hInstance, int nCmdShow);

    /// <summary>	Puntero a la interfaz gráfica. </summary>
    model3D* interfaz_grafica;

private:
	/// <summary>	Información actual de las manos. </summary>
	Mano mano_izd;
	Mano mano_der;

	/// <summary>	Estado actual del sistema. </summary>
	AutomataEstados automata_estados;

	/// <summary>	Gesto actual que se está realizando. </summary>
	AccionGesto* gesto_actual;
	

    /// <summary>	Handle of the window. </summary>
    HWND                    m_hWnd;
    /// <summary>	The start time. </summary>
    INT64                   m_nStartTime;
    /// <summary>	The last counter. </summary>
    INT64                   m_nLastCounter;
    /// <summary>	The frequency. </summary>
    double                  m_fFreq;
    /// <summary>	The next status time. </summary>
    INT64                   m_nNextStatusTime;
    /// <summary>	The frames since update. </summary>
    DWORD                   m_nFramesSinceUpdate;

    /// <summary>	The kinect sensor. </summary>
    IKinectSensor*          m_pKinectSensor;
    /// <summary>	The coordinate mapper. </summary>
    ICoordinateMapper*      m_pCoordinateMapper;


    /// <summary>	The body frame reader. </summary>
    IBodyFrameReader*       m_pBodyFrameReader;

    /// <summary>	The d 2D factory. </summary>
    ID2D1Factory*           m_pD2DFactory;

    // Body/hand drawing.
    /// <summary>	The render target. </summary>
    ID2D1HwndRenderTarget*  m_pRenderTarget;
    /// <summary>	The brush joint tracked. </summary>
    ID2D1SolidColorBrush*   m_pBrushJointTracked;
    /// <summary>	The brush joint inferred. </summary>
    ID2D1SolidColorBrush*   m_pBrushJointInferred;
    /// <summary>	The brush bone tracked. </summary>
    ID2D1SolidColorBrush*   m_pBrushBoneTracked;
    /// <summary>	The brush bone inferred. </summary>
    ID2D1SolidColorBrush*   m_pBrushBoneInferred;
    /// <summary>	The brush hand closed. </summary>
    ID2D1SolidColorBrush*   m_pBrushHandClosed;
    /// <summary>	The brush hand open. </summary>
    ID2D1SolidColorBrush*   m_pBrushHandOpen;
    /// <summary>	The brush hand lasso. </summary>
    ID2D1SolidColorBrush*   m_pBrushHandLasso;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Main processing function. </summary>
    ///
    /// <remarks>	Mike, 19/12/2019. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void                    Update();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Initializes the default Kinect sensor. </summary>
    ///
    /// <remarks>	Mike, 19/12/2019. </remarks>
    ///
    /// <returns>	S_OK on success, otherwise failure code. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    HRESULT                 InitializeDefaultSensor();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Handle new body data. </summary>
    ///
    /// <remarks>	Mike, 19/12/2019. </remarks>
    ///
    /// <param name="nTime">	 	timestamp of frame. </param>
    /// <param name="nBodyCount">	body data count. </param>
    /// <param name="ppBodies">  	[in,out] body data in frame. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void                    ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Set the status bar message. </summary>
    ///
    /// <remarks>	Mike, 19/12/2019. </remarks>
    ///
    /// <param name="szMessage">		[in,out] message to display. </param>
    /// <param name="nShowTimeMsec">	time in milliseconds for which to ignore future status
    /// 								messages. </param>
    /// <param name="bForce">			force status update. </param>
    ///
    /// <returns>	True if it succeeds, false if it fails. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool                    SetStatusMessage(_In_z_ WCHAR* szMessage, DWORD nShowTimeMsec, bool bForce);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Ensure necessary Direct2d resources are created. </summary>
    ///
    /// <remarks>	Mike, 19/12/2019. </remarks>
    ///
    /// <returns>	S_OK if successful, otherwise an error code. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    HRESULT EnsureDirect2DResources();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Dispose Direct2d resources. </summary>
    ///
    /// <remarks>	Mike, 19/12/2019. </remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void DiscardDirect2DResources();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Converts a body point to screen space. </summary>
    ///
    /// <remarks>	Mike, 19/12/2019. </remarks>
    ///
    /// <param name="bodyPoint">	body point to tranform. </param>
    /// <param name="width">		width (in pixels) of output buffer. </param>
    /// <param name="height">   	height (in pixels) of output buffer. </param>
    ///
    /// <returns>	point in screen-space. </returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    D2D1_POINT_2F           BodyToScreen(const CameraSpacePoint& bodyPoint, int width, int height);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Draws a body. </summary>
    ///
    /// <remarks>	Mike, 19/12/2019. </remarks>
    ///
    /// <param name="pJoints">	   	joint data. </param>
    /// <param name="pJointPoints">	joint positions converted to screen space. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void                    DrawBody(const Joint* pJoints, const D2D1_POINT_2F* pJointPoints);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>
    /// Draws a hand symbol if the hand is tracked: red circle = closed, green circle = opened; blue
    /// circle = lasso.
    /// </summary>
    ///
    /// <remarks>	Mike, 19/12/2019. </remarks>
    ///
    /// <param name="handState">   	state of the hand. </param>
    /// <param name="handPosition">	position of the hand. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void                    DrawHand(HandState handState, const D2D1_POINT_2F& handPosition);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Draws one bone of a body (joint to joint) </summary>
    ///
    /// <remarks>	Mike, 19/12/2019. </remarks>
    ///
    /// <param name="pJoints">	   	joint data. </param>
    /// <param name="pJointPoints">	joint positions converted to screen space. </param>
    /// <param name="joint0">	   	one joint of the bone to draw. </param>
    /// <param name="joint1">	   	other joint of the bone to draw. </param>
    ///
    /// ### <param name="pJointPoints">	joint positions converted to screen space. </param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void                    DrawBone(const Joint* pJoints, const D2D1_POINT_2F* pJointPoints, JointType joint0, JointType joint1);
};

