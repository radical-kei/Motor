//
//  MotorApp.swift
//  MotorApp
//
//  Created by radical on 2021/04/04.
//

import SwiftUI

@main
struct MotorApp: App {
    @Environment(\.scenePhase) var scenePhase
    var object: Motor
    
    var body: some Scene {
        WindowGroup {
            ContentView()
                .environmentObject(object)
        }
        
        .onChange(of: scenePhase) { phase in
            if phase == .background {
                object.to_background()
            }
            if phase == .active {
                object.to_active()
            }
            if phase == .inactive {
            }
        }
    }
    
    init(){
        object = Motor(host: "192.168.1.43",port: "50001")
    }
}
