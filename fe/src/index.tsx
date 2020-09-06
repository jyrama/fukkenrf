import * as React from 'react';
import * as ReactDOM from 'react-dom';
import {observable, configure, action} from 'mobx';
import {observer} from 'mobx-react';

import { Radio, RadioChangeEvent, Divider, message } from "./CRE";

configure({enforceActions: 'always'});


enum HF {
    LOVHF1 = 54,
    LOVHF2 = 2,
    HF1 = 5,
    HF2 = 6,
    HF3 = 7,
    HF4 = 8,
    HF5 = 9,
    HF6 = 11
}

enum VHF {
    VHF1 = 12,
    VHF2 = 13,
    VHF3 = 14,
    VHF4 = 15,
    VHF5 = 16,
    VHF6 = 17
}

// const HFENABLEPIN = 5;

class AppState {
    
    @observable hfSelection: HF;
    @observable vhfSelection: VHF;

    constructor() {
    }

    @action.bound
    setHF(selection: HF) {
        this.hfSelection = selection;
    }

    @action.bound
    setVHF(selection: VHF) {
        this.vhfSelection = selection;
    }
}

@observer
class TimerView extends React.Component<{appState: AppState}, {}> {
    render() {
        return (
            <div>
                <Divider>LoVHF / HF selection</Divider>
                <Radio.Group buttonStyle="solid" onChange={this.changeHF}>
                    <Radio.Button value={HF.LOVHF1}>LoVHF1</Radio.Button>
                    <Radio.Button value={HF.LOVHF2}>LoVHF2</Radio.Button>
                    <Radio.Button value={HF.HF1}>HF1</Radio.Button>
                    <Radio.Button value={HF.HF2}>HF2</Radio.Button>
                    <Radio.Button value={HF.HF3}>HF3</Radio.Button>
                    <Radio.Button value={HF.HF4}>HF4</Radio.Button>
                    <Radio.Button value={HF.HF5}>HF5</Radio.Button>
                    <Radio.Button value={HF.HF6}>HF6</Radio.Button>
                </Radio.Group>
                
                <Divider>VHF selection</Divider>
                <Radio.Group buttonStyle="solid" onChange={this.changeVHF}>
                    <Radio.Button value={VHF.VHF1}>VHF1</Radio.Button>
                    <Radio.Button value={VHF.VHF2}>VHF2</Radio.Button>
                    <Radio.Button value={VHF.VHF3}>VHF3</Radio.Button>
                    <Radio.Button value={VHF.VHF4}>VHF4</Radio.Button>
                    <Radio.Button value={VHF.VHF5}>VHF5</Radio.Button>
                    <Radio.Button value={VHF.VHF6}>VHF6</Radio.Button>
                </Radio.Group>
                <br></br>
                <br></br>
                <br></br>
                HF: {this.props.appState.hfSelection}
                <br></br>
                VHF: {this.props.appState.vhfSelection}
            </div>
        );
     }

     @action.bound
     async changeHF(e: RadioChangeEvent) {
         this.props.appState.setHF(e.target.value);
         const res = await fetch(`http://192.168.9.4/`, {
            method: "POST",
            headers: {},
            body: String.fromCharCode(Number(e.target.value) + 65)
            });
        if (res.ok) {
            message.success("HF change sent!");
        }
     }

     @action.bound
     async changeVHF(e: RadioChangeEvent) {
        this.props.appState.setVHF(e.target.value);
        const res = await fetch(`http://192.168.9.4/`, {
            method: "POST",
            headers: {},
            body: String.fromCharCode(Number(e.target.value) + 65)
            });
        if (res.ok) {
            message.success("VHF change sent!");
        }
     }
};

const appState = new AppState();
ReactDOM.render(<TimerView appState={appState} />, document.getElementById('root'));
