//
//  ContentView.swift
//  MotorApp
//
//  Created by radical on 2021/04/04.
//

import SwiftUI


struct ContentView: View{
    @EnvironmentObject var object: Motor

    var body: some View {
        VStack{
            Text("モーター速度")
            .font(.title)
            Text("\(Int(object.speed))%")

            Slider(
                value: $object.speed,
                in: -100...100,
                step: 1,
                minimumValueLabel: Text("-100"),
                maximumValueLabel: Text("100")
            ) {
                Text("Speed")
            }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        Group {
            ContentView()
        }
    }
}
