# IronSource SDK

This is a native extension for [Defold engine](http://www.defold.com) with partial implementation of allows to use [IronSource iOS SDK](https://developers.ironsrc.com/ironsource-mobile/ios/ios-sdk/#step-1), [IronSource Android SDK](https://developers.ironsrc.com/ironsource-mobile/android/android-sdk/#step-1) 

ATTENTION! Not all API methods are fully supported; see LUA Api section below for the list of supported methods. Only Interstitials and Rewardeds are supported, no Banners support.

Used IronSource SDK version 6.15.0 both for Android and iOS 

If you want to use IS as a mediation service, you need to add iOS SDKs and adapters for other ad networks by your own. But this should not require any changes in the extension code.

## Installation

You can use the IronSource extension in your own project by adding this project as a [Defold library dependency](http://www.defold.com/manuals/libraries/).
Open your game.project file and in the dependencies field under project add:

>https://github.com/MaratGilyazov/def_ironsource/archive/master.zip

For iOS you also need to modify your info.plist as said here: https://developers.ironsrc.com/ironsource-mobile/ios/ios-sdk/#step-1
Add this to your Defold info.plist template: 
```lua
<key>NSAppTransportSecurity</key>  
<dict>  
	<key>NSAllowsArbitraryLoads</key><true/>  
</dict>
```

## Example
```lua
if ironsource then
	ironsource.set_consent(true)
	ironsource.init("your_app_key")
	ironsource.set_callback(ironsource_callback)
	ironsource.validate_integration()
	ironsource.load_interstitial()
end

if ironsource and ironsource.is_interstitial_ready() then
	ironsource.show_interstitial()
end

if ironsource and ironsource.is_rewarded_ready() then
	ironsource.show_rewarded()
end

function ironsource_callback(self, message_type, message)
	if message_type == ironsource.INTERSTITIAL_DID_LOAD then
		print("ironsource_callback: INTERSTITIAL_DID_LOAD")
	elseif message_type == ironsource.INTERSTITIAL_DID_SHOW then
		print("ironsource_callback: INTERSTITIAL_DID_SHOW")
	elseif message_type == ironsource.INTERSTITIAL_DID_CLICK then
		print("ironsource_callback: INTERSTITIAL_DID_CLICK")
	elseif message_type == ironsource.INTERSTITIAL_DID_CLOSE then
		print("ironsource_callback: INTERSTITIAL_DID_CLOSE")
		ironsource.load_interstitial()
	elseif message_type == ironsource.INTERSTITIAL_DID_OPEN then
		print("ironsource_callback: INTERSTITIAL_DID_OPEN")
	elseif message_type == ironsource.INTERSTITIAL_DID_FAIL_TO_SHOW then
		print("ironsource_callback: INTERSTITIAL_DID_FAIL_TO_SHOW")
		ironsource.load_interstitial()
	elseif message_type == ironsource.INTERSTITIAL_DID_FAIL_TO_LOAD then
		print("ironsource_callback: INTERSTITIAL_DID_FAIL_TO_LOAD")
		
	elseif message_type == ironsource.REWARDED_AVAILABLE then
		print("ironsource_callback: REWARDED_AVAILABLE")
	elseif message_type == ironsource.REWARDED_NOT_AVAILABLE then
		print("ironsource_callback: REWARDED_NOT_AVAILABLE")
	elseif message_type == ironsource.REWARDED_DID_COMPLETE then
		print("ironsource_callback: REWARDED_DID_COMPLETE")
	elseif message_type == ironsource.REWARDED_DID_FAIL then
		print("ironsource_callback: REWARDED_DID_FAIL")
	elseif message_type == ironsource.REWARDED_DID_OPEN then
		print("ironsource_callback: REWARDED_DID_OPEN")
	elseif message_type == ironsource.REWARDED_DID_CLOSE then
		print("ironsource_callback: REWARDED_DID_CLOSE")
	elseif message_type == ironsource.REWARDED_DID_CLICK then
		print("ironsource_callback: REWARDED_DID_CLICK")
	elseif message_type == ironsource.REWARDED_DID_START then
		print("ironsource_callback: REWARDED_DID_START")
	elseif message_type == ironsource.REWARDED_DID_END then
		print("ironsource_callback: REWARDED_DID_END")
	end
end
```

## LUA Api
#### ironsource.init(string app_key, bool gdpr_consent)
Initializes IronSource, call this before any other calls to IS. 

app_key - is your APP_KEY from the IS dashboard

gdpr_consent [optional parameter] - boolean flag to forward your user's consent regarding sensitive data; send true for "OptIn", if permission are granted; it's an optional parameter, you can call "set_consent()" method instead
#### ironsource.set_consent(bool consent)
Set your user's consent regarding sensitive data. Call this before "init()" method, or send consent with "init(key, consent)" call
consent - boolean flag to forward your user's consent regarding sensitive data; send true for "OptIn", if permission are granted
#### ironsource.set_callback(function ironsource_callback)
ironsource_callback(self, message_type, message) - callback for all types of IS delegate methods, see the example with all events listed. "message" table is always empty in this implementation
#### ironsource.validate_integration()
Prints info to iOS consloe to validate your integration. More info here: https://developers.ironsrc.com/ironsource-mobile/ios/integration-helper-ios/
#### ironsource.load_interstitial()
Preloads interstitial ads, call it before showing interstitials and either wait for ironsource.INTERSTITIAL_DID_LOAD event or check manually if ironsource.is_interstitial_ready().
ATTENTION! You need to load interstitial ad each time when the previous ad was shown and closed. 
#### ironsource.is_interstitial_ready()
Returns true if the interstitial ad is ready, false - otherwise
#### ironsource.show_interstitial()
Shows interstitial ad
#### ironsource.is_rewarded_ready()
Returns true if the rewarded ad is ready, false - otherwise. 
#### ironsource.show_rewarded()
Shows interstitial ad. Before showing rewarded ad either wait for ironsource.REWARDED_AVAILABLE event or check manually if ironsource.is_rewarded_ready(). There is no way to preload rewarded ads manually - instead of interstitials it's automated by SDK.

## SDK support level
Some APIs are not supported in this version of extention, see the full list of native SDK methods in the [IronSource iOS SDK repo](https://developers.ironsrc.com/ironsource-mobile/ios/ios-sdk/). 

Feel free to push a Pull Request with other features implementation.