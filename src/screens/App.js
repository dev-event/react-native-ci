import React, {Component} from 'react';
import {
  SafeAreaView,
  View,
  Text,
  NativeModules,
  TouchableOpacity,
} from 'react-native';
import Card from '../component/Card';
import styles from '../styles';
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
    this.handleRequestModule = this.handleRequestModule.bind(this);

    this.state = {
      //constant
      id: null,
      text: null,
      //error log
      error: null,
      //function params
      sampleString: null,
      sampleTwice: null,
      sampleCallbackString: null,
      sampleCallbackLater: null,
      sampleAsAsync: null,
      sampleAsPromise: null,
    };
  }

  handleRequestModule = async () => {
    const {error} = this.state;
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
    let getHello = NativeModules.CustomCxxModule.hello();
    this.setState({sampleString: getHello})

    NativeModules.CustomCxxModule.getString(result => {
      this.setState({sampleCallbackString: result});
    });

    NativeModules.CustomCxxModule.call_later(2000, result => {
      this.setState({sampleCallbackLater: result});
    });

    let getTwice = NativeModules.CustomCxxModule.twice(2);
    this.setState({sampleTwice: getTwice})

    // NativeModules.CustomCxxModule.addIfPositiveAsPromise(0, (error, result) => {
    //   this.setState({sampleAsPromise: result});
    // });
    //
    // NativeModules.CustomCxxModule.addIfPositiveAsAsync(0, (error, result) => {
    //   this.setState({sampleAsAsync: result});
    // });

    NativeModules.CustomCxxModule.getEvent();
  };

  render() {
    const {id, text} = this.state;
    const {
      error,
      sampleCallbackString,
      sampleCallbackLater,
      sampleTwice,
      sampleString,
      sampleAsPromise,
      sampleAsAsync,
    } = this.state;
    return (
      <SafeAreaView style={styles.container}>
        <TouchableOpacity
          style={styles.button}
          onPress={this.handleRequestModule}>
          <Text style={styles.h2}>Run Script</Text>
        </TouchableOpacity>
        {!error ? (
          <View>
            <Card title="const id" description={id} />
            <Card title="const text" description={text} />
            <Card title="hello" description={sampleString} />
            <Card title="callback" description={sampleCallbackString} />
            <Card title="callback later" description={sampleCallbackLater} />
            <Card title="twice" description={sampleTwice} />
            <Card title="addIfPositiveAsPromise" description={sampleAsPromise} />
            <Card title="addIfPositiveAsAsync" description={sampleAsAsync} />
          </View>
        )
         : (
          <Text>{error}</Text>
        )
        }
      </SafeAreaView>
    );
  }
}

export default App;
