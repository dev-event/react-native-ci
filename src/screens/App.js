import React, {PureComponent} from 'react';
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

const Bridge = NativeModules.CustomCxxModule;

class App extends PureComponent {
  constructor() {
    super();
    this.handleRequestModule = this.handleRequestModule.bind(this);
    this.state = {
      id: null,
      text: null,
      error: null,
      sampleAdd: null,
      sampleTwice: null,
      sampleString: null,
      sampleAsAsync: null,
      sampleAsPromise: null,
      sampleCallbackLater: null,
      sampleCallbackString: null,
    };
  }

  handleRequestModule = async () => {
    const {error} = this.state;
    try {
      await this.getConstantModule();
      await this.getFunctionModule();
    } catch (errors) {
      this.setState({error: errors || 'Error callback in C++ Native Module'});
    }
  };

  getConstantModule = () => {
    //* TODO get constant Native Modules
    this.setState({
      id: Bridge.constId || 'not found',
      text: Bridge.constText || 'not found',
    });
  };

  getFunctionModule = () => {
    //* TODO get Methods Native Modules
    this.setState({
      sampleTwice: Bridge.twice(2) || 'not found',
      sampleString: Bridge.hello() || 'not found',
    });

    Bridge.getString(result => {
      this.setState({sampleCallbackString: result || 'not found'});
    });

    Bridge.call_later(2000, (result, error) => {
      this.setState({sampleCallbackLater: result || 'not found'});
    });

    Bridge.add(10, 13, (result, error) => {
      this.setState({sampleAdd: result || 'not found'});
    });

    Bridge.getEvent();

    //* TODO Need fixing params

    // Bridge.addIfPositiveAsPromise(0, (result, error) => {
    //   this.setState({sampleAsPromise: result || 'not found'});
    // });
    //
    // Bridge.addIfPositiveAsAsync(0, (result, error) => {
    //   this.setState({sampleAsAsync: result || 'not found'});
    // });
  };

  render() {
    const {id, text} = this.state;
    const {
      error,
      sampleAdd,
      sampleTwice,
      sampleString,
      sampleAsAsync,
      sampleAsPromise,
      sampleCallbackLater,
      sampleCallbackString,
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
            <Card title="twice" description={sampleTwice} />
            <Card title="add" description={sampleAdd} />
            <Card title="hello" description={sampleString} />
            <Card title="callback" description={sampleCallbackString} />
            <Card title="callback later" description={sampleCallbackLater} />
            <Card title="addIfPositiveAsAsync" description={sampleAsAsync} />
            <Card title="addIfPositiveAsPromise" description={sampleAsPromise} />
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
