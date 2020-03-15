import {StyleSheet } from 'react-native';

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    width: '100%',
    padding: 16,
  },
  h2: {
    color: 'dodgerblue',
    fontSize: 18,
    marginTop: 8,
  },
  button: {
    fontSize: 18,
    marginTop: 8,
    padding: 8,
    justifyContent: 'center',
  },
  content: {
    color: 'black',
    marginBottom: 14,
    marginTop: 8,
    width: 300
  },
  containerText: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    padding: 4,
    height: 40,
  },
  contentText: {
    flexDirection: 'column',
    alignItems: 'center'
  },
});

export default styles;
