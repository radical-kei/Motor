//
//  Motor.swift
//  Motor
//
//  Created by radical on 2021/06/20.
//

import Foundation
import Network
import SwiftUI

class Motor : ObservableObject{
    
    var addTimer: Timer!
    var g_connection : NWConnection!
    var g_active = false
    var g_host: NWEndpoint.Host
    var g_port: NWEndpoint.Port

    @Published var speed = 0.0
    
    func to_background()
    {
        if g_active == true {
            g_connection.cancel()
            g_connection = nil
            addTimer.invalidate()
            addTimer = nil
            g_active = false
        }
    }
    
    func to_active()
    {
        if g_active == false {
            connect()
            addTimer = Timer.scheduledTimer(timeInterval:0.2,
                                            target:self,
                                            selector:#selector(self.interval_send),
                                            userInfo:nil,
                                            repeats:true)
            g_active = true
        }
    }
    
    func send()
    {
        let message = String(Int(speed * 10000))
        let data = message.data(using: .utf8)!
        let semaphore = DispatchSemaphore(value: 0)

        NSLog(message)
        g_connection.send(content: data, completion: .contentProcessed { error in
            if let error = error {
                NSLog("\(#function), \(error)")
            } else {
                semaphore.signal()
            }
        })

        semaphore.wait()
    }
    
    func connect() -> Int
    {
        let semaphore = DispatchSemaphore(value: 0)
        var ret = -1
    
        g_connection = NWConnection(host: g_host,
                                    port: g_port,
                                    using: .tcp)
        g_connection.stateUpdateHandler = { (newState) in
            switch newState {
                case .ready:
                    NSLog("Ready to send")
                    ret = 0
                case .waiting(let error):
                    NSLog("\(#function), \(error)")
                case .failed(let error):
                    NSLog("\(#function), \(error)")
                case .setup: break
                case .cancelled: break
                case .preparing: break
                @unknown default:
                    fatalError("Illegal state")
            }
            semaphore.signal()
        }
        
        let queue = DispatchQueue(label: "motor")
        g_connection.start(queue:queue)
        semaphore.wait()
        return ret
    }
    
    @objc func interval_send(sender: Timer)
    {
        send()
    }
    
    init(host: String, port: String) {
        NSLog("init")
        g_host = NWEndpoint.Host(host)
        g_port = NWEndpoint.Port(port) ?? NWEndpoint.Port(50001)
        to_active()
    }
    
    deinit{
        g_connection.cancel()
        g_connection = nil
        addTimer.invalidate()
        addTimer = nil
    }
}
