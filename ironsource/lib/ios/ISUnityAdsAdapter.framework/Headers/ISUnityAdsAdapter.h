//
//  Copyright (c) 2015 IronSource. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IronSource/ISBaseAdapter+Internal.h"

static NSString * const UnityAdsAdapterVersion = @"4.1.8";
static NSString * GitHash = @"64a2e850b";

//System Frameworks For UnityAds Adapter

@import AdSupport;
@import CoreTelephony;
@import StoreKit;

@interface ISUnityAdsAdapter : ISBaseAdapter

@end
