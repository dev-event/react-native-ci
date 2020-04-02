import React, {memo} from 'react';
import {View, Text} from 'react-native';
import styles from '../styles';

const Card = memo(
  ({title, description}) =>
    title && (
      <View style={styles.content}>
        <View style={styles.containerText}>
          <View style={styles.contentText}>
            <Text numberOfLines={1} style={styles.h2}>
              {title}
            </Text>
          </View>
          <Text style={styles.desc}>{description}</Text>
        </View>
      </View>
    ),
);

export default Card;
