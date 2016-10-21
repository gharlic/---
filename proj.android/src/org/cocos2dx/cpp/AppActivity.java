/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;

import com.pumpai.pumpai.R;
import com.pumpai.pumpai.R.string;
import com.kakao.kakaolink.AppActionBuilder; 
import com.kakao.kakaolink.KakaoLink;
import com.kakao.kakaolink.KakaoTalkLinkMessageBuilder;
import com.kakao.util.KakaoParameterException;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.Bundle;
import android.util.Log;

 public class AppActivity extends Cocos2dxActivity { 
 	static AppActivity currentContext; 
 	
 	private KakaoLink kakaoLink;
 	private KakaoTalkLinkMessageBuilder kakaoBuilder;
 	 
 	// jni로 쓰기 위함 
 	public static Activity actInstance; 
 	 
 	 
 	@Override 
 	public void onCreate(Bundle savedInstanceState) { 
 		currentContext = this; 
 		super.onCreate(savedInstanceState); 
 		 
 		// jni로 사용하기 위함 
 		actInstance = this;
 		try {
 		kakaoLink = KakaoLink.getKakaoLink(this);
		kakaoBuilder = kakaoLink.createKakaoTalkLinkMessageBuilder();
 		} catch(KakaoParameterException e) {
 			e.printStackTrace();
 			Log.d("cocos2d-x", "error1");
 		}
 	} 
 	 
 	// jni로 사용하기 위함 
 	public static Object getJavaActivity() 
 	{ 
 		return actInstance; 
 	} 


 	public void kakaoLink(String str) {

 		try { 
 			String text = str;
 			kakaoBuilder.addText(text).build();
 			kakaoLink.sendMessage(kakaoBuilder, this);
 			kakaoBuilder = kakaoLink.createKakaoTalkLinkMessageBuilder();
 			
 		} catch (KakaoParameterException e) { 
 			e.printStackTrace();
 			Log.d("cocos2d-x", "error2");
 		} 
 		
 		
 	} 
 } 
