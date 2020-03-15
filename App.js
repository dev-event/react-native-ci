import React, {Component} from 'react';
import {
  SafeAreaView,
  View,
  Text,
  NativeModules,
  TouchableOpacity,
} from 'react-native';
import Card from './src/component/Card';
import styles from './src/styles';
import MessageQueue from 'react-native/Libraries/BatchedBridge/MessageQueue';

const spyFunction = msg => {
  if (msg.module === 'RCTDeviceEventEmitter') {
    console.log(msg);
  }
};

MessageQueue.spy(spyFunction);

class App extends Component {
  constructor() {
    super();
    this.state = {
      error: null,
      id: null,
      text: null,
      callbackString: null,
      twice: null,
    };
  }

  handleRequestModule = async () => {
    const { error } = this.state;
    try {
      await this.getConstantModule();
      await this.getFunctionModule();
    } catch (err) {
      this.setState({error: err || 'Error callback in C++ Native Module'});
    }
  };

  getConstantModule = () => {
    //* TODO get constant Native Modules

    let getId = NativeModules.CustomCxxModule.constId;
    let getText = NativeModules.CustomCxxModule.constText;
    this.setState({id: getId, text: getText});
  };

  getFunctionModule = () => {
    //* TODO get Methods Native Modules 

    NativeModules.CustomCxxModule.getString(result => {
      this.setState({callbackString: result});
    });
    let getTwice = NativeModules.CustomCxxModule.twice(2);
    NativeModules.CustomCxxModule.getEvent();
    this.setState({twice: getTwice});
  };

  render() {
    const {id, text} = this.state;
    const {error, callbackString, twice} = this.state;
    return (
      <SafeAreaView style={styles.container}>
        <TouchableOpacity
          style={styles.button}
          onPress={() => {
            this.handleRequestModule();
          }}>
          <Text style={styles.h2}>Run Script</Text>
        </TouchableOpacity>
        <View>
          <Card title="const id" description={id} />
          <Card title="const text" description={text} />

          <Card title="callback" description={callbackString} />
          <Card title="double func" description={twice} />
        </View>
        <Text>{error}</Text>
      </SafeAreaView>
    );
  }
}

export default App;
